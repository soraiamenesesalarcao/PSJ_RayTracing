#include "RayTracer.h"

RayTracer::RayTracer() {
}

RayTracer * RayTracer::getInstance(){
	static RayTracer instance;
	return &instance;
}


/*Algoritmo ray tracing*/
RGB RayTracer::trace(RGB * background, std::vector<Light> lights, std::vector<Object*> objects,Ray ray, int depth, float ior){
	RGB color;

	glm::vec3 closestPi, closestNormal;
	
	glm::vec2 lightAttenuation(0.06f, 0.06f);

	//determine a interseção mais próxima com um objeto
	Object* objectIntersect = closestIntersection(objects, closestPi, closestNormal, ray);
	if(objectIntersect == NULL){

		color.r = background->r; color.g = background->g; color.b = background->b;

	} else {
		float diffuseR = 0, diffuseG = 0, diffuseB = 0;
		float specularR = 0, specularG = 0, specularB = 0;
		float niu1, niu2;
		glm::vec3 V, L, R, N, Vt;

		glm::vec3 auxDir = glm::vec3(ray.direction.x * 0.001, ray.direction.y * 0.001, ray.direction.z * 0.001); 
		V = glm::normalize(Camera::getInstance()->computeV());
		N = glm::normalize(closestNormal);

		for(int l = 0; l < lights.size(); l++){
			//L = vector unitário na direção do point para a posição da luz;
			L = glm::normalize(lights[l].getPosition() - closestPi);

			//inicializacao do shadow feeler
			Ray shadowFeeler;
			shadowFeeler.origin = closestPi - auxDir;
			shadowFeeler.direction = L;
			
			//vector reflexao especular
			R = glm::normalize(2 * glm::dot(V, N) * N - V); 

			float LdotN = std::max(glm::dot(L, N), 0.0f); //para o calculo do material
			float attenuation;

			if(LdotN > 0) {		
				// se intersecta com um shadow feeler
				glm::vec3 aux;
				if(closestIntersection(objects, aux, aux, shadowFeeler) !=NULL){
					continue;
				}
				attenuation = 1.0f / (1.0f + lightAttenuation.x * glm::length(closestPi - lights[l].getPosition()) + lightAttenuation.y * pow(glm::length(closestPi -  lights[l].getPosition()), 2.0f));
				// Componente difusa

				diffuseR += objectIntersect->getMaterial().getKd() * objectIntersect->getMaterial().getColor().r * lights[l].getColor().r * LdotN;
				diffuseG += objectIntersect->getMaterial().getKd() * objectIntersect->getMaterial().getColor().g * lights[l].getColor().g * LdotN;
				diffuseB += objectIntersect->getMaterial().getKd() * objectIntersect->getMaterial().getColor().b * lights[l].getColor().b * LdotN;

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
			//RGB * background, std::vector<Light> lights, std::vector<Object*> objects,Ray ray, int depth, float ior
			// Calculo da reflexao
			if(objectIntersect->getMaterial().getKs() > 0){
				Ray reflectionRay = computeReflectionRay(closestPi, R);
				RGB reflectionColor = trace(background, lights, objects, reflectionRay, depth + 1, ior);

				color.r += reflectionColor.r * objectIntersect->getMaterial().getKs();
				color.g += reflectionColor.g * objectIntersect->getMaterial().getKs();
				color.b += reflectionColor.b * objectIntersect->getMaterial().getKs();
			}
		
			// Refraction
			if(objectIntersect->getMaterial().getT() != 0) {
				Vt = glm::dot(-ray.direction, N) * N + ray.direction;
				float newIOR;
				Ray refractionRay = computeRefractionRay(closestPi, Vt, N, ior, objectIntersect->getMaterial().getIndexRefraction(), &newIOR);
				if(refractionRay.origin.x != NULL) {
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

Ray RayTracer::computeReflectionRay(glm::vec3 Pi, glm::vec3 r){
	Ray ray;
	ray.origin = Pi + (0.001f * r);
	ray.direction = r;
	return ray;
}

Ray RayTracer::computeRefractionRay(glm::vec3 Pi, glm::vec3 Vt, glm::vec3 N, float ior, float iorObject, float * newIOR){
	Ray ray;
	float sinThetaI, sinThetaT, cosThetaT;

	if(ior != 1.0f) 
		*newIOR = 1.0f;
	else *newIOR = iorObject;

	glm::vec3 t = glm::normalize(Vt); 
	glm::vec3 rt;

	sinThetaI = glm::sqrt(glm::pow2(Vt.x) + glm::pow2(Vt.y) + glm::pow2(Vt.z));
	sinThetaT = (ior/(*newIOR)) * sinThetaI;

	float sinQuad = sinThetaT*sinThetaT;

	cosThetaT = 0.0f;
	if(sinQuad <= 1){
		cosThetaT = glm::sqrt(1.0f - sinQuad);

		rt = sinThetaT * t + cosThetaT*(-N);

		ray.origin = Pi + (0.001f * rt);
		ray.direction = glm::normalize(rt);
	
	}	
	else ray.origin.x = NULL;
	return  ray;	
}


/*		// Compute the secondary rays
		if(depth < MAX_DEPTH){
			// Calculo da reflexao
			
			if(material.ks > 0){
				Ray reflectionRay = computeReflectionRay(closestPi, R);
				RGB reflectionColor = trace(nff, reflectionRay, depth + 1, ior);

				material.color.r += reflectionColor.r * material.ks;
				material.color.g += reflectionColor.g * material.ks;
				material.color.b += reflectionColor.b * material.ks;
			}
		
			// Refraction
			if(material.t != 0) {
				Vt = glm::dot(-ray.direction, N) * N + ray.direction;
				float newIOR;
				Ray refractionRay = computeRefractionRay(closestPi, Vt, N, ior, material.indexRefraction, &newIOR);
				if(refractionRay.origin.x != NULL) {
					RGB refractionColor = trace(nff, refractionRay, depth + 1, newIOR);
					material.color.r += refractionColor.r * material.t;
					material.color.g += refractionColor.g * material.t;
					material.color.b += refractionColor.b * material.t;
				}

			}
		}
	}

	return material.color;
}

float RayTracer::isLeft(glm::vec2 P0, glm::vec2 P1, glm::vec2 P2)
{
    return ((P1.x - P0.x) * (P2.y - P0.y) - (P2.x -  P0.x) * (P1.y - P0.y));
}

int RayTracer::windingNumber(std::vector<glm::vec2> vertices, glm::vec2 point) {
	int wn = 0;
	std::vector<glm::vec2>::iterator v;
	std::vector<glm::vec2>::iterator v1;
	
	for(v = vertices.begin(), v1 = vertices.begin() + 1; v != vertices.end() - 1; v++, v1++) {
		if (v->y <= point.y) {
			if (v1->y  > point.y) {  
				if (isLeft( *v, *v1, point) > 0) {
					++wn;     
				}
			}
		}
		else {                        
			if (v1->y  <= point.y) {
				if (isLeft( *v, *v1, point) < 0) { 
					--wn;                  
				}
			}
		}
	}
	return wn;
}

bool RayTracer::polygonContainsPoint(	glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, 
										glm::vec3 normal, glm::vec3 point) {
	glm::vec2 projectedV1, projectedV2, projectedV3, projectedPoint;
	glm::vec3 absNormal = glm::vec3(std::abs(normal.x), std::abs(normal.y), std::abs(normal.z));
	float axisToIgnore = std::max(std::max(absNormal.x, absNormal.y), absNormal.z);
	// 2D Projection
	if(axisToIgnore == absNormal.x) {
		projectedV1 = glm::vec2(v1.y, v1.z);
		projectedV2 = glm::vec2(v2.y, v2.z);
		projectedV3 = glm::vec2(v3.y, v3.z);
		projectedPoint = glm::vec2(point.y, point.z);
	}
	else if(axisToIgnore == absNormal.y) {
		projectedV1 = glm::vec2(v1.x, v1.z);
		projectedV2 = glm::vec2(v2.x, v2.z);
		projectedV3 = glm::vec2(v3.x, v3.z);
		projectedPoint = glm::vec2(point.x, point.z);
	}
	else {
		projectedV1 = glm::vec2(v1.x, v1.y);
		projectedV2 = glm::vec2(v2.x, v2.y);
		projectedV3 = glm::vec2(v3.x, v3.y);
		projectedPoint = glm::vec2(point.x, point.y);
	}

	// Winding Number Test
	std::vector<glm::vec2> vertices;
	vertices.push_back(projectedV1);
	vertices.push_back(projectedV2);
	vertices.push_back(projectedV3);
	vertices.push_back(projectedV1);
	int wn = windingNumber(vertices, projectedPoint);		
	if (wn != 0)
		return true;
	else
		return false;
}

bool RayTracer::intersectPolygonAux(glm::vec3 * Pi, float * Ti, glm::vec3 * normal, Ray ray, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3){

	glm::vec3 N = glm::cross((v2 - v1), (v3 - v1));

	// calcular o ponto de intersecao com o plano do poligono
	float NdotD = glm::dot(N, ray.direction);
	if(NdotD == 0){
		return false;
	}

	glm::vec3 sub = ray.origin - v1;
	float NdotO = glm::dot(N, sub);
	float t = - (NdotO/ NdotD);
	if(t < 0){
		return false;
	}	
	*Pi = ray.origin + ray.direction*t;

	// ver se o ponto de intersecao com o plano esta dentro do poligono
	if (polygonContainsPoint(v1, v2,v3, N, *Pi)) {
		*Ti = t;
		*normal = glm::normalize(N);
		return true;
	}
	return false; 
}

bool RayTracer::intersect(glm::vec3 * Pi, float * Ti, glm::vec3 * normal, ConeCylinder coneCylinder, Ray ray){
	float A, B, C;
	// cylinder 
	if(coneCylinder.apex_radius == coneCylinder.base_radius) { 
		A = glm::pow2(ray.direction.x) + glm::pow2(ray.direction.y);
		B = (2 * ray.direction.x * ray.origin.x)  + (2 * ray.direction.y * ray.origin.y);
		C = glm::pow2(ray.origin.x) + glm::pow2(ray.origin.y) - 1;		
	}
	// cone
	else { 
		A = glm::pow2(ray.direction.x) + glm::pow2(ray.direction.y) - glm::pow2(ray.direction.z);
		B = (2 * ray.direction.x * ray.origin.x)  + (2 * ray.direction.y * ray.origin.y) - (2 * ray.direction.z * ray.origin.z);
		C = glm::pow2(ray.origin.x) + glm::pow2(ray.origin.y) - glm::pow2(ray.origin.z);
			
	}
	float disc = glm::pow2(B) - 4*A*C;
	float t1 = (-B - glm::sqrt(disc))/(2*A);
	float t2 = (-B + glm::sqrt(disc))/(2*A);
		
	float z1 = ray.origin.z + t1 * ray.direction.z;
	float z2 = ray.origin.z + t2 * ray.direction.z;

	if((z1 > coneCylinder.base_position.pz && z1 < coneCylinder.apex_position.pz) 
			|| (z2 > coneCylinder.base_position.pz && z2 < coneCylinder.apex_position.pz)) {
		if(z1 < z2) {
			// TO DO: calculate normals
			*Pi = ray.origin + t1 * ray.direction;
			*Ti = t1;
		}
		else {
			// TO DO: calculate normals
			*Pi = ray.origin + t2 * ray.direction;
			*Ti = t2;
		}
		return true;
	}
	else return false;
	return false;
}
*/

