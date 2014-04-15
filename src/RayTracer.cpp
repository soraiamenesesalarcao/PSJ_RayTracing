#include "RayTracer.h"

RayTracer::RayTracer() {
	_usingGrid = false;
}

RayTracer::~RayTracer() {
	
}

void RayTracer::init(std::vector<Object*> objects) {
	_grid.computeBoundingBoxes(objects);
	_grid.addObjectsToGrid(objects);
}


/*Algoritmo ray tracing*/
RGB RayTracer::trace(RGB * background, std::vector<Light> lights, std::vector<Object*> objects, Ray ray, int depth, float ior, glm::vec3 V){
	
	
	RGB color;
	glm::vec3 closestPi, closestNormal;
	float closestTi;
	glm::vec2 lightAttenuation(0.06f, 0.06f);
	Object* objectIntersect; 

	//determina a interseccao mais proxima com um objeto
	if(_usingGrid) {
		objectIntersect = closestIntersectionGrid(objects, closestPi, closestTi, closestNormal, ray);
		//std::cout << "[Grid] Objecto " << objectIntersect->getMaterial().getColor().r << std::endl;
	}
	else {
		objectIntersect = closestIntersection(objects, closestPi, closestTi, closestNormal, ray);
	}

	if(objectIntersect == NULL){
		color.r = background->r; color.g = background->g; color.b = background->b;
	} 
	else {
		float diffuseR = 0, diffuseG = 0, diffuseB = 0;
		float specularR = 0, specularG = 0, specularB = 0;
		float newLightR = 0, newLightG = 0, newLightB = 0, gridLightR, gridLightG, gridLightB;
		glm::vec3 L, R, N, Vt, newLightPosition, newL, newOrigin;
  
		N = glm::normalize(closestNormal);

		for(std::size_t l = 0; l < lights.size(); l++){

			newLightR = 0.0;
			newLightG = 0.0;
			newLightB = 0.0;
			//Vector unitario na direccao do point para a posicao da luz;
			L = glm::normalize(lights[l].getPosition() - closestPi);

			//inicializacao do shadow feeler
			Ray shadowFeeler;
	
			//vector reflexao especular
			R = glm::normalize(2 * glm::dot(V, N) * N - V); 

			float LdotN = std::max(glm::dot(L, N), 0.0f); //para o calculo do material
			float attenuation;

			gridLightR = lights[l].getColor().r / (LIGHT_SIDE * LIGHT_SIDE);
			gridLightG = lights[l].getColor().g / (LIGHT_SIDE * LIGHT_SIDE);
			gridLightB = lights[l].getColor().b / (LIGHT_SIDE * LIGHT_SIDE);

			if(LdotN > 0) {	

				for(int x = 0; x < LIGHT_SIDE; x++) {
					for(int y = 0; y < LIGHT_SIDE; y++) {

						newLightPosition.x = lights[l].getPosition().x + x *  (rand() % 10) * LIGHT_EPSILON * LIGHT_GRID_RATIO;
						newLightPosition.y = lights[l].getPosition().y;
						newLightPosition.z = lights[l].getPosition().z + y *  (rand() % 10) * LIGHT_EPSILON * LIGHT_GRID_RATIO;

						newL = glm::normalize(newLightPosition - closestPi);

						newOrigin = closestPi + newL * EPSILON;

						shadowFeeler.setOrigin(newOrigin);
						shadowFeeler.setDirection(newL);

						// se intersecta com um shadow feeler
						glm::vec3 aux;
						float auxT;
						if(closestIntersection(objects, aux, auxT, aux, shadowFeeler) != NULL) {
							continue;					
						}
						newLightR += gridLightR;
						newLightG += gridLightG;
						newLightB += gridLightB;
					}
				}

				attenuation = 1.0f / (1.0f + lightAttenuation.x * glm::length(closestPi - lights[l].getPosition()) 
									       + lightAttenuation.y * pow(glm::length(closestPi -  lights[l].getPosition()), 
										   2.0f));				
				// Componente difusa
				diffuseR += objectIntersect->getMaterial().getKd() * objectIntersect->getMaterial().getColor().r * newLightR * LdotN;
				diffuseG += objectIntersect->getMaterial().getKd() * objectIntersect->getMaterial().getColor().g * newLightG * LdotN;
				diffuseB += objectIntersect->getMaterial().getKd() * objectIntersect->getMaterial().getColor().b * newLightB * LdotN;

				

				//componente especular
				float RdotL = std::max(glm::dot(R,L), 0.0f);
				if(RdotL > 0){ 
					specularR += objectIntersect->getMaterial().getKs() * objectIntersect->getMaterial().getColor().r * 
									lights[l].getColor().r * glm::pow(RdotL, objectIntersect->getMaterial().getShine()) * attenuation;
					specularG += objectIntersect->getMaterial().getKs() * objectIntersect->getMaterial().getColor().g * 
									lights[l].getColor().g * glm::pow(RdotL, objectIntersect->getMaterial().getShine()) * attenuation;
					specularB += objectIntersect->getMaterial().getKs() * objectIntersect->getMaterial().getColor().b * 
									lights[l].getColor().b * glm::pow(RdotL, objectIntersect->getMaterial().getShine()) * attenuation;
				}
			}//end if(LdotN > 0)
		}
		
		color.r = diffuseR + specularR;
		color.g = diffuseG + specularG;
		color.b = diffuseB + specularB;

		// Compute the secondary rays
		if(depth < MAX_DEPTH){

			// Calculo da reflexao
			if(objectIntersect->getMaterial().getKs() > 0){
				Ray reflectionRay;
				reflectionRay.computeReflectedRay(closestPi, R);
				RGB reflectionColor = trace(background, lights, objects, reflectionRay, depth + 1, ior, V);
				color.r += reflectionColor.r * objectIntersect->getMaterial().getKs();
				color.g += reflectionColor.g * objectIntersect->getMaterial().getKs();
				color.b += reflectionColor.b * objectIntersect->getMaterial().getKs();
			}
		
			// Refraction
			if(objectIntersect->getMaterial().getT() != 0) {
				Vt = glm::dot(-ray.getDirection(), N) * N + ray.getDirection();
				float newIOR;
				Ray refractionRay;
				refractionRay.computeRefractedRay(closestPi, Vt, N, ior, objectIntersect->getMaterial().getIndexRefraction(), &newIOR);
				if(refractionRay.getOrigin().x != NULL) {
					RGB refractionColor = trace(background, lights, objects, refractionRay, depth + 1, newIOR, V);
					color.r += refractionColor.r * objectIntersect->getMaterial().getT();
					color.g += refractionColor.g * objectIntersect->getMaterial().getT();
					color.b += refractionColor.b * objectIntersect->getMaterial().getT();
				}
			}

		} //end if depth
	}

	return color;
}

Object* RayTracer::closestIntersection(std::vector<Object*> objects, glm::vec3 &Pi, float &Ti, glm::vec3 &normal, Ray ray){
	bool hasIntersectedGlobal = false;
	bool hasIntersectedLocal = false;

	Object* closestObject = NULL;

	glm::vec3 closestPi, closestNormal;
	float tempTi, closestTi = FLT_MAX;

	for(std::size_t i = 0; i < objects.size(); i++){
		hasIntersectedLocal = objects[i]->intersect(&closestPi, &tempTi, &closestNormal, ray);
		if(hasIntersectedLocal) {
			if(!hasIntersectedGlobal) {
				closestObject = objects[i];
				closestTi = tempTi;
				Pi = closestPi;
				Ti = closestTi;
				normal = closestNormal;
				hasIntersectedGlobal = true;
			}
			if(tempTi < closestTi) {
				closestObject = objects[i];
				closestTi = tempTi;
				Pi = closestPi;
				Ti = closestTi;
				normal = closestNormal;
			}
		}
	}
	return closestObject;
}


Object* RayTracer::closestIntersectionGrid(std::vector<Object*> objects, glm::vec3 &Pi, float &Ti, glm::vec3 &normal, Ray ray){

	Object* closestObject = NULL;
	glm::vec3 rayTmax, rayTmin;
	float tMax;
	Cell * startingCell, * intersectionCell;
	int stepX, stepY, stepZ;
	// Check if the ray intersects the Grid's BB
	bool hasIntersectedGrid = _grid.getBoundingBox().intersect(ray, &rayTmin, &rayTmax);

	if(hasIntersectedGrid) {
		
		// 1) take the starting cell:
		// - the one where the ray origin is
		// OR
		// - the one where the ray enters the grid

		// 4) tMaxX, tMaxY = ray->box intersection
		startingCell = _grid.getStartingCell(ray, rayTmin);
				

		// 3) if ray.direction.x < 0 => stepX = -1 else stepX = 1; same for y
		stepX = (ray.getDirection().x >= 0) ? 1 : -1;
		stepY = (ray.getDirection().y >= 0) ? 1 : -1;
		stepZ = (ray.getDirection().z >= 0) ? 1 : -1;

		// 2), 5) and 6)
		intersectionCell = _grid.cellTraversal(startingCell, &tMax, &rayTmin, &rayTmax, stepX, stepY, stepZ);
		// compute closest intersection inside the grid
		closestObject = closestIntersection(intersectionCell->getObjects(), Pi, Ti, normal, ray);
		while(Ti > tMax) {
			intersectionCell = _grid.cellTraversal(intersectionCell, &tMax, &rayTmin, &rayTmax, stepX, stepY, stepZ);	
			closestObject = closestIntersection(intersectionCell->getObjects(), Pi, Ti, normal, ray);
		}
	} 
	return closestObject;
}


void RayTracer::toggleUsingGrid() {
	_usingGrid = (_usingGrid) ? false : true;
}