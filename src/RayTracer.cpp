#include "RayTracer.h"

RayTracer::RayTracer() {
}

RayTracer * RayTracer::getInstance(){
	static RayTracer instance;
	return &instance;
}


/*Algoritmo ray tracing*/
RGB RayTracer::trace(RGB * background, std::vector<Light> lights, std::vector<Object*> objects, Ray ray, int depth, float ior){
	
	
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
		glm::vec3 V, L, R, N, Vt, newLightPosition, newL, newOrigin;
  
		V = glm::normalize(Camera::getInstance()->computeV());
		N = glm::normalize(closestNormal);

		for(int l = 0; l < lights.size(); l++){

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
				RGB reflectionColor = trace(background, lights, objects, reflectionRay, depth + 1, ior);
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
					RGB refractionColor = trace(background, lights, objects, refractionRay, depth + 1, newIOR);
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

	for(int i = 0; i < objects.size(); i++){
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