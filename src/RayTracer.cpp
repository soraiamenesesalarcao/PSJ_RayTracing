#include "RayTracer.h"

RayTracer::RayTracer() {
	_usingGrid = false;
	_nRays = 0;
}

RayTracer::~RayTracer() {
	
}

int RayTracer::getNRays() {
	return _nRays;
}

void RayTracer::incNRays() {
	_nRays++;
}

void RayTracer::clearNRays() {
	_nRays = 0;
}

void RayTracer::init(std::vector<Object*> objects) {

	if(_usingGrid) {
		_grid.computeBoundingBoxes(objects);
		_grid.addObjectsToGrid(objects);
	}

	for(std::size_t i = 0; i < objects.size(); i++){	
		objects[i]->setLastRayID(0);
	}
	clearNRays();
}


/*Algoritmo ray tracing*/
RGB RayTracer::trace(RGB * background, std::vector<Light> lights, std::vector<Object*> objects, 
						Ray * ray, int depth, float ior, glm::vec3 V){		
	RGB color;	
	glm::vec2 lightAttenuation(0.06f, 0.06f);

	Object* objectIntersect = NULL;
	
	Material objMaterial;
	glm::vec3 closestPi, closestNormal;
	float closestTi;

	//determina a interseccao mais proxima com um objeto
	if(_usingGrid) {
		Cell * startingCell;
		glm::vec3 rayTmax, rayTmin, rayCellPoint, rayMax;
		glm::vec3 rayDelta;
		float tDist = FLT_MAX, tProx = -FLT_MAX;
		bool hasIntersectedGrid;
	
		// Check if the ray intersects the Grid's BB
		hasIntersectedGrid = _grid.getBoundingBox().intersect(*ray, &rayTmin, &rayTmax, &tProx, &tDist, &rayCellPoint);

		if(hasIntersectedGrid) {

			startingCell = _grid.getStartingCell(*ray, rayCellPoint);

			rayDelta.x = (rayTmax.x - rayTmin.x) / _grid.getN().x;
			rayDelta.y = (rayTmax.y - rayTmin.y) / _grid.getN().y;
			rayDelta.z = (rayTmax.z - rayTmin.z) / _grid.getN().z;

			rayMax.x = (ray->getDirection().x >= 0)	? (rayTmin.x + (startingCell->getX() + 1) * rayDelta.x) 
													: (rayTmax.x - (startingCell->getX()) * rayDelta.x);

			rayMax.y = (ray->getDirection().y >= 0)	? (rayTmin.y + (startingCell->getY() + 1) * rayDelta.y)
													: (rayTmax.y - (startingCell->getY()) * rayDelta.y);

			rayMax.z = (ray->getDirection().z >= 0)	? (rayTmin.z + (startingCell->getZ() + 1) * rayDelta.z)
													: (rayTmax.z - (startingCell->getZ()) * rayDelta.z);

			objectIntersect = cellTraversal(startingCell, rayDelta, rayMax, ray);
		} 
	}
	else {
		objectIntersect = closestIntersection(objects, ray);
	}

	if(objectIntersect == NULL){
		color.r = background->r; color.g = background->g; color.b = background->b;
	} 
	else {
		objMaterial = ray->getWinnerMaterial();
		closestPi = ray->getWinnerPi();
		closestTi = ray->getWinnerTi();
		closestNormal = ray->getWinnerNormal();

		float diffuseR = 0, diffuseG = 0, diffuseB = 0;
		float specularR = 0, specularG = 0, specularB = 0;
		float newLightR = 0, newLightG = 0, newLightB = 0, gridLightR, gridLightG, gridLightB;
		glm::vec3 L, R, N, Vt, newLightPosition, newL, newOrigin;
 
		N = glm::normalize(closestNormal);

		glm::vec3 aux;
		float auxT;
		
		for(std::size_t l = 0; l < lights.size(); l++){
			
			newLightR = 0.0;
			newLightG = 0.0;
			newLightB = 0.0;

			//Vector unitario na direccao do point para a posicao da luz;
			L = glm::normalize(lights[l].getPosition() - closestPi);

			//inicializacao do shadow feeler
			Ray shadowFeeler;
			incNRays();
			shadowFeeler.setRayID(getNRays());
	
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
						incNRays();
						shadowFeeler.setRayID(getNRays());

						// se intersecta com um shadow feeler

						Object * object2 = NULL;

						if(_usingGrid) {
							glm::vec3 rayTmax, rayTmin, rayCellPoint, rayMax;
							glm::vec3 rayDelta;
							float tDist = FLT_MAX, tProx = -FLT_MAX;
							bool hasIntersectedGrid;
							Cell * startingCell;
	
							// Check if the ray intersects the Grid's BB
							hasIntersectedGrid = _grid.getBoundingBox().intersect(shadowFeeler, &rayTmin, &rayTmax, &tProx, &tDist, &rayCellPoint);

							if(hasIntersectedGrid) {

								startingCell = _grid.getStartingCell(shadowFeeler, rayCellPoint);

								rayDelta.x = (rayTmax.x - rayTmin.x) / _grid.getN().x;
								rayDelta.y = (rayTmax.y - rayTmin.y) / _grid.getN().y;
								rayDelta.z = (rayTmax.z - rayTmin.z) / _grid.getN().z;

								rayMax.x = (shadowFeeler.getDirection().x >= 0)	? (rayTmin.x + (startingCell->getX() + 1) * rayDelta.x) 
																		: (rayTmax.x - (startingCell->getX()) * rayDelta.x);

								rayMax.y = (shadowFeeler.getDirection().y >= 0)	? (rayTmin.y + (startingCell->getY() + 1) * rayDelta.y)
																		: (rayTmax.y - (startingCell->getY()) * rayDelta.y);

								rayMax.z = (shadowFeeler.getDirection().z >= 0)	? (rayTmin.z + (startingCell->getZ() + 1) * rayDelta.z)
																		: (rayTmax.z - (startingCell->getZ()) * rayDelta.z);

								object2 = cellTraversal(startingCell, rayDelta, rayMax, &shadowFeeler);
							} 
						}
						else {
							object2 = closestIntersection(objects, &shadowFeeler);
						}

						if (object2 != NULL) {	
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
				diffuseR += objMaterial.getKd() * objMaterial.getColor().r * newLightR * LdotN;				
				diffuseG += objMaterial.getKd() * objMaterial.getColor().g * newLightG * LdotN;
				diffuseB += objMaterial.getKd() * objMaterial.getColor().b * newLightB * LdotN;

				//componente especular
				float RdotL = std::max(glm::dot(R,L), 0.0f);
				if(RdotL > 0){ 
					specularR += objMaterial.getKs() * objMaterial.getColor().r * 
									lights[l].getColor().r * glm::pow(RdotL, objMaterial.getShine()) * attenuation;
					specularG += objMaterial.getKs() * objMaterial.getColor().g * 
									lights[l].getColor().g * glm::pow(RdotL, objMaterial.getShine()) * attenuation;
					specularB += objMaterial.getKs() * objMaterial.getColor().b * 
									lights[l].getColor().b * glm::pow(RdotL, objMaterial.getShine()) * attenuation;
				}
			}//end if(LdotN > 0)
		}
		
		color.r = diffuseR + specularR;
		color.g = diffuseG + specularG;
		color.b = diffuseB + specularB;

		// Compute the secondary rays
		if(depth < MAX_DEPTH){
			// Reflection
			if(objMaterial.getKs() > 0){
				Ray reflectionRay;
				reflectionRay.computeReflectedRay(closestPi, R);
				incNRays();
				reflectionRay.setRayID(getNRays());
				RGB reflectionColor = trace(background, lights, objects, &reflectionRay, depth + 1, ior, V);
				color.r += reflectionColor.r * objMaterial.getKs();
				color.g += reflectionColor.g * objMaterial.getKs();
				color.b += reflectionColor.b * objMaterial.getKs();
			}
		
			// Refraction
			if(objMaterial.getT() != 0) {
				Vt = glm::dot(-ray->getDirection(), N) * N + ray->getDirection();
				float newIOR;
				Ray refractionRay;
				refractionRay.computeRefractedRay(closestPi, Vt, N, ior, objMaterial.getIndexRefraction(), &newIOR);
				incNRays();
				refractionRay.setRayID(getNRays());
				if(refractionRay.getOrigin().x != NULL) {
					RGB refractionColor = trace(background, lights, objects, &refractionRay, depth + 1, newIOR, V);
					color.r += refractionColor.r * objMaterial.getT();
					color.g += refractionColor.g * objMaterial.getT();
					color.b += refractionColor.b * objMaterial.getT();
				}
			}

		} //end if depth
	}
	
	return color;
}


Object * RayTracer::cellTraversal (Cell * startingCell, glm::vec3 rayDelta, glm::vec3 rayMax, Ray * ray) {
	
	int iX, iY, iZ, stepX, stepY, stepZ, stopX, stopY, stopZ;
	glm::vec3 rayDir = ray->getDirection();	
	
	Cell * intersectedCell = NULL;
	Object * intersectedObject = NULL;
	
	iX = startingCell->getX();
	iY = startingCell->getY();
	iZ = startingCell->getZ();

	stepX = (rayDir.x >= 0) ? 1 : -1;
	stepY = (rayDir.y >= 0) ? 1 : -1;
	stepZ = (rayDir.z >= 0) ? 1 : -1;

	stopX = (rayDir.x >= 0) ? _grid.getN().x : -1;
	stopY = (rayDir.y >= 0) ? _grid.getN().y : -1;
	stopZ = (rayDir.z >= 0) ? _grid.getN().z : -1;

	// Incremental phase
	while(true) {

		intersectedCell = _grid.getCell(iX, iY, iZ);

		if(!intersectedCell->isEmpty()) {
			std::vector<Object *> objs = intersectedCell->getObjects();
			intersectedObject = closestIntersection(intersectedCell->getObjects(), ray);
		}

		if(rayMax.x < rayMax.y) {
			if(rayMax.x < rayMax.z) {
				if(intersectedObject != NULL && ray->getWinnerTi() < rayMax.x) { 
					return intersectedObject;
				}
				iX += stepX;
				if(iX == stopX) {
					return NULL;
				}
				rayMax.x += rayDelta.x; 
			}
			else {
				if(intersectedObject != NULL && ray->getWinnerTi() < rayMax.z) { 
					return intersectedObject;
				}
				iZ += stepZ;
				if(iZ == stopZ) {
					return NULL;
				}
				rayMax.z += rayDelta.z;
			}
		}
		else {
			if(rayMax.y < rayMax.z) {

				if(intersectedObject != NULL && ray->getWinnerTi() < rayMax.y) { 
					return intersectedObject;
				}
				iY += stepY;
				if(iY == stopY) {
					return NULL;
				}
				rayMax.y += rayDelta.y;
			}
			else {
				if(intersectedObject != NULL && ray->getWinnerTi() < rayMax.z) { 
					return intersectedObject;
				}
				iZ += stepZ;
				if(iZ == stopZ) {
					return NULL;
				}
				rayMax.z += rayDelta.z;
			}
		} 
	}
}


Object* RayTracer::closestIntersection(std::vector<Object*> objects, Ray * ray){
	bool hasIntersectedGlobal = false;
	bool hasIntersectedLocal = false;

	Object* closestObject = NULL;

	glm::vec3 closestPi, closestNormal;
	float tempTi, closestTi = FLT_MAX;

	for(std::size_t i = 0; i < objects.size(); i++){	
		
	// Verificacao para multiplas intersecoes entre o mesmo raio e o mesmo objecto
	//	if(objects[i]->getLastRayID() == 0 || !ray->equalTo(objects[i]->getLastRayID())) {

			hasIntersectedLocal = objects[i]->intersect(&closestPi, &tempTi, &closestNormal, *ray);		

			if(hasIntersectedLocal) {
				if(!hasIntersectedGlobal || (tempTi < closestTi)) {
					closestTi = tempTi;
					objects[i]->setLastRayID(ray->getRayID());
					objects[i]->setLastTi(closestTi);
					closestObject = objects[i];
					hasIntersectedGlobal = true;
					ray->setWinnerData(closestObject->getMaterial(), closestPi, closestNormal, closestTi);
				}
			}

		// Verificacao para multiplas intersecoes entre o mesmo raio e o mesmo objecto
		/*}
		else if (ray->equalTo(objects[i]->getLastRayID()) 
			&& (!hasIntersectedGlobal || objects[i]->getLastTi() < closestTi)) {
			hasIntersectedGlobal = true;			
			closestTi = objects[i]->getLastTi();
			closestObject = objects[i];
			ray->setWinnerData(closestObject->getMaterial(), closestPi, closestNormal, closestTi);
		}*/
	}	

	return closestObject;
}


void RayTracer::toggleUsingGrid() {
	if(_usingGrid) {
		_usingGrid = false;
		std::cout << "Grid desactivada" << std::endl;
	}
	else {
		_usingGrid = true;
		std::cout << "Grid activada" << std::endl;
	}
}

void RayTracer::setUsingDoF(bool dof) {
	_usingDoF = dof;
}

/* Monte Carlo Sampling!!! */
RGB RayTracer::monteCarlo(Camera camera, RGB * background, std::vector<Light> lights, std::vector<Object*> objects, float x, float y, int depth){
	RGB color;

	glm::vec2 position[4];
	float margin = 1.0f / (2.0f * depth); 
	position[0] = glm::vec2(x+margin, y+margin); // top right
	position[1] = glm::vec2(x-margin, y+margin); // top left
	position[2] = glm::vec2(x-margin, y-margin); // bottom left
	position[3] = glm::vec2(x+margin, y-margin); // bottom right

	glm::vec3 colors[4];
	colors[0] = glm::vec3(0.0f, 0.0f, 0.0f);
	colors[1] = glm::vec3(0.0f, 0.0f, 0.0f);
	colors[2] = glm::vec3(0.0f, 0.0f, 0.0f);
	colors[3] = glm::vec3(0.0f, 0.0f, 0.0f);

	// It starts by tracing four rays at the corners of each pixel
	for(int i = 0; i < 4; i++){
		Ray ray = camera.PrimaryRay(position[i].x, position[i].y);
		incNRays();
		ray.setRayID(getNRays());
		if(_usingDoF)
			color = depthOfField(background, lights, objects, &ray, 1, 1.0f, glm::normalize(camera.computeV()),camera);
		else
			color = trace(background, lights, objects, &ray, 1, 1.0f, glm::normalize(camera.computeV()));
		
		colors[i] = glm::vec3(color.r, color.g, color.b);
	}

	//If the colors are similar (check the threshold) then just use their average
	for(int i = 0, j = 1; i < 4 ; i++, j++){

		if(i == 3) j = 0;
		float diff = glm::abs(colors[i].r - colors[j].r) + glm::abs(colors[i].g - colors[j].g) + glm::abs(colors[i].b - colors[j].b);
		if( diff > THRESHOLD){ //different colors
			if(depth < 4){
				monteCarlo(camera, background, lights, objects,position[i].x, position[i].y, depth+1);
			}
			break;
		}
	}

	color.r = color.g = color.b = 0.0f;
	for(int r = 0; r < 4; r++){
		color.r += colors[r].x;
		color.g += colors[r].y;
		color.b += colors[r].z;
	}

	color.r /= 4; color.g /= 4; color.b /= 4;

	return color;
}

/* Depth of Field*/
RGB RayTracer::depthOfField(RGB * background, std::vector<Light> lights, std::vector<Object*> objects, Ray * ray, int depth, float ior, glm::vec3 V, Camera camera){
	RGB color;

	glm::vec3 rayDirection = ray->getDirection();
	incNRays();
	ray->setRayID(getNRays());
	glm::vec3 P = ray->getOrigin() + (float)FOCAL_LENGHT * rayDirection; //camera.getEye()

	RGB colorT;

	float r = 0.5f; //radius of disc
	color.r = 0.0f; color.g = 0.0f; color.b = 0.0f;

	for(int i = 0; i<DEPTH_RAYS; i++){
		float du = (float)rand()/float(RAND_MAX + 1);
		float dv = (float)rand()/float(RAND_MAX + 1);

		// creating new camera position(or ray start using jittering) //camera.getEye()
		glm::vec3 ls = ray->getOrigin() - (r / 2.0f)*camera.getXe() - (r / 2.0f)*camera.getYe() + r*(du)*camera.getXe() + r*(dv)*camera.getYe();

		//glm::vec3 ls = camera.getEye() - camera.getXe()*du*r - camera.getYe()*dv*r;

		glm::vec3 direction = glm::normalize(P - ls);

		ray->setDirection(direction);
		ray->setOrigin(ls);

		colorT = trace(background, lights, objects, ray, 1, 1.0f, V);
		color.r += colorT.r;
		color.g += colorT.g;
		color.b += colorT.b;
	}

	color.r = color.r/((float) DEPTH_RAYS);
	color.g = color.g/((float) DEPTH_RAYS);
	color.b = color.b/((float) DEPTH_RAYS);

	return color;
}