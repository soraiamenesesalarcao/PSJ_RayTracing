#include "RayTracer.h"

RayTracer::RayTracer() {
	_usingGrid = false;
}

RayTracer::~RayTracer() {
}


/*Algoritmo ray tracing*/
RGB RayTracer::trace(RGB * background, std::vector<Light> lights, std::vector<Object*> objects, Ray ray, int depth, float ior, glm::vec3 V){
	
	
	RGB color;
	glm::vec3 closestPi, closestNormal;
	glm::vec2 lightAttenuation(0.06f, 0.06f);

	//determina a interseccao mais proxima com um objeto
	Object* objectIntersect = closestIntersection(objects, closestPi, closestNormal, ray);
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
						if(closestIntersection(objects, aux, aux, shadowFeeler) != NULL) {
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

Object* RayTracer::closestIntersection(std::vector<Object*> objects, glm::vec3 &Pi, glm::vec3 &normal, Ray ray){
	bool hasIntersectedGlobal = false;
	bool hasIntersectedLocal = false;

	Object* closestObject = NULL;

	glm::vec3 closestPi, closestNormal;
	float Ti, closestTi = FLT_MAX;

	for(std::size_t i = 0; i < objects.size(); i++){
		hasIntersectedLocal = objects[i]->intersect(&closestPi, &Ti, &closestNormal, ray);
		if(hasIntersectedLocal) {
			if(!hasIntersectedGlobal) {
				closestObject = objects[i];
				closestTi = Ti;
				Pi = closestPi;
				normal = closestNormal;
				hasIntersectedGlobal = true;
			}
			if(Ti < closestTi) {
				closestObject = objects[i];
				closestTi = Ti;
				Pi = closestPi;
				normal = closestNormal;
			}
		}
	}
	return closestObject;
}


Object* RayTracer::closestIntersectionGrid(std::vector<Object*> objects, glm::vec3 &Pi, glm::vec3 &normal, Ray ray){
	bool hasIntersectedGlobal = false;
	bool hasIntersectedLocal = false;
	bool hasIntersectedGrid = false;

	Object* closestObject = NULL;

	glm::vec3 closestPi, closestNormal;
	float Ti, closestTi = FLT_MAX;

	// Check if the ray intersects the Grid's BB
	hasIntersectedGrid = _grid.getBoundingBox().intersect(ray);
	if(hasIntersectedGrid) {
		
		// TO DO

	} 
	return closestObject;
}


void RayTracer::computeBoundingBoxes(std::vector<Object*> objects) {
	glm::vec3 pMin, pMax;

	// init
	objects[0]->setBoundingBox();
	pMin = objects[0]->getBoundingBox().getPosMin();
	pMax = objects[0]->getBoundingBox().getPosMax();

	for(std::size_t i = 1; i < objects.size(); i++){
		objects[i]->setBoundingBox();

		pMin.x = std::min(objects[i]->getBoundingBox().getPosMin().x, pMin.x);
		pMin.y = std::min(objects[i]->getBoundingBox().getPosMin().y, pMin.y);
		pMin.z = std::min(objects[i]->getBoundingBox().getPosMin().z, pMin.z);

		pMax.x = std::max(objects[i]->getBoundingBox().getPosMax().x, pMax.x);
		pMax.y = std::max(objects[i]->getBoundingBox().getPosMax().y, pMax.y);
		pMax.z = std::max(objects[i]->getBoundingBox().getPosMax().z, pMax.z);	
				
		pMin.x -= EPSILON;
		pMin.y -= EPSILON;
		pMin.z -= EPSILON;

		pMax.x += EPSILON;
		pMax.y += EPSILON;
		pMax.z += EPSILON;

		_grid.setBoundingBox(pMin.x, pMin.y, pMin.z, pMax.x, pMax.y, pMax.z);
	}
}


void RayTracer::addObjectsToGrid(std::vector<Object*> objects) {
	glm::vec3 obbMin, obbMax, gbbMin, N = _grid.getN(), W = _grid.getW();
	int iMinX, iMinY, iMinZ, iMaxX, iMaxY, iMaxZ, x, y, z;

	gbbMin = _grid.getBoundingBox().getPosMin();
	_grid.setCells(objects.size(), MULTIPLY_FACTOR);

	for(std::size_t i = 1; i < objects.size(); i++){
		obbMin = objects[i]->getBoundingBox().getPosMin();
		obbMax = objects[i]->getBoundingBox().getPosMax();
		
		iMinX = glm::clamp(((obbMin.x - gbbMin.x) * N.x) / W.x, 0.0f, N.x - 1);
		iMinY = glm::clamp(((obbMin.y - gbbMin.y) * N.y) / W.y, 0.0f, N.y - 1);
		iMinZ = glm::clamp(((obbMin.z - gbbMin.z) * N.z) / W.z, 0.0f, N.z - 1);

		iMaxX = glm::clamp(((obbMax.x - gbbMin.x) * N.x) / W.x, 0.0f, N.x - 1);
		iMaxY = glm::clamp(((obbMax.y - gbbMin.y) * N.y) / W.y, 0.0f, N.y - 1);
		iMaxZ = glm::clamp(((obbMax.z - gbbMin.z) * N.z) / W.z, 0.0f, N.z - 1);
				
		for(x = iMinX; x < iMaxX; x++) {
			for(y = iMinY; y < iMaxY; y++) {
				for(z = iMinZ; z < iMaxZ; z++) {
					_grid.getCell(x, y, z)->addObject(objects[i]);
				}
			}
		}

	}	
}


void RayTracer::toggleUsingGrid() {
	_usingGrid = (_usingGrid) ? false : true;
}