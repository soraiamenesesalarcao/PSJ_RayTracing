#include "RayTracer.h"

RayTracer::RayTracer() {
}

RayTracer * RayTracer::getInstance(){
	static RayTracer instance;
	return &instance;
}


/*
 * Verificacao de interseccoes para os raios shadows!!! 
 */
bool RayTracer::intersecta(NFF *nff, Ray ray){
	bool hasIntersectedGlobal = false;
	bool hasIntersectedLocal = false;

	glm::vec3 Pi;
	glm::vec3 normal;
	float Ti;

	for(std::vector<Plan>::iterator pl = nff->planes.begin(); pl != nff->planes.end(); pl++) {
		hasIntersectedLocal = intersect(&Pi, &Ti, &normal, *pl, ray);
		if(hasIntersectedLocal) {
			return true;
		}
	}

	for(std::vector<Sphere>::iterator s = nff->spheres.begin(); s != nff->spheres.end(); s++) {
		hasIntersectedLocal = intersect(&Pi, &Ti, &normal, *s, ray);
		if(hasIntersectedLocal) {
			return true;
		}
	}

	for(std::vector<Polygon>::iterator p = nff->polygons.begin(); p != nff->polygons.end(); p++){
		hasIntersectedLocal = intersect(&Pi, &Ti, &normal, *p, ray);
		if(hasIntersectedLocal) {
			return true;
		}
	}

	return false;
}

RGB RayTracer::trace(NFF * nff, Ray ray, int depth, float ior){
	bool hasIntersectedGlobal = false;
	bool hasIntersectedLocal = false;
	glm::vec3 Pi;
	glm::vec3 closestPi;
	glm::vec3 normal;
	glm::vec3 closestNormal;
	float Ti;
	float closestTi = FLT_MAX;
	bool IsInside = false;

	glm::vec2 lightAttenuation(0.06f, 0.06f);

	// Default material
	Material material;
	material.color = nff->background;
	material.indexRefraction = 1.0;
	material.kd = 0.0;
	material.ks = 0.0;
	material.shine = 0.0;
	material.t = 0.0;

	// Check if there is an intersection
	for(std::vector<Plan>::iterator pl = nff->planes.begin(); pl != nff->planes.end(); pl++) {
		hasIntersectedLocal = intersect(&Pi, &Ti, &normal, *pl, ray);
		if(hasIntersectedLocal) {
			if(!hasIntersectedGlobal) {
				closestTi = Ti;
				closestPi = Pi;
				closestNormal = normal;
				material = pl->mtl;
				hasIntersectedGlobal = true;
			}
			if(Ti < closestTi) {
				closestTi = Ti;
				closestPi = Pi;
				closestNormal = normal;
				material = pl->mtl;
			}
		}

	}

	for(std::vector<Polygon>::iterator p = nff->polygons.begin(); p != nff->polygons.end(); p++) {
		hasIntersectedLocal = intersect(&Pi,  &Ti, &normal, *p, ray);
		if(hasIntersectedLocal) {
			if(!hasIntersectedGlobal) {
				closestTi = Ti;
				closestPi = Pi;
				closestNormal = normal;
				material = p->mtl;
				hasIntersectedGlobal = true;
			}
			if(Ti < closestTi) {
				closestTi = Ti;
				closestPi = Pi;
				closestNormal = normal;
				material = p->mtl;
			}
		}
	}
	for(std::vector<PolygonPatch>::iterator pp = nff->polygonPatchs.begin(); pp != nff->polygonPatchs.end(); pp++) {
		hasIntersectedLocal = intersect(&Pi,  &Ti, &normal, *pp, ray);
		if(hasIntersectedLocal) {
			if(!hasIntersectedGlobal) {
				closestTi = Ti;
				closestPi = Pi;
				closestNormal = normal;
				material = pp->mtl;
				hasIntersectedGlobal = true;
			}
			if(Ti < closestTi) {
				closestTi = Ti;
				closestPi = Pi;
				closestNormal = normal;
				material = pp->mtl;
			}
		}

	}
	for(std::vector<Sphere>::iterator s = nff->spheres.begin(); s != nff->spheres.end(); s++) {
		hasIntersectedLocal = intersect(&Pi, &Ti, &normal, *s, ray);

		if(hasIntersectedLocal) {		
			if(!hasIntersectedGlobal) {
				closestTi = Ti;
				closestPi = Pi;
				closestNormal = normal;
				material = s->mtl;
				hasIntersectedGlobal = true;
			}
			if(Ti < closestTi) {
				closestTi = Ti;
				closestPi = Pi;
				closestNormal = normal;
				material = s->mtl;
			}
		}
	}

	// COMENTADO PORQUE FALTA CALCULAR NORMAIS NO INTERSECT !!!
	//for(std::vector<ConeCylinder>::iterator c = nff->coneAndCylinders.begin(); c != nff->coneAndCylinders.end(); c++) {
	//	hasIntersectedLocal = intersect(&Pi,  &Ti, &normal, *c, ray);
	//	if(hasIntersectedLocal) {
	//		if(!hasIntersectedGlobal) {
	//			closestTi = Ti;
	//			closestPi = Pi;
	//			closestNormal = normal;
	//			material = c->mtl;
	//			hasIntersectedGlobal = true;
	//		}
	//		if(Ti < closestTi) {
	//			closestTi = Ti;
	//			closestPi = Pi;
	//			closestNormal = normal;
	//			material = c->mtl;
	//		}
	//	}
	//}


	// Check ih there was an intersection
	if(hasIntersectedGlobal) {
		 float diffuseR = 0, diffuseG = 0, diffuseB = 0;
		 float specularR = 0, specularG = 0, specularB = 0;
		 float niu1, niu2;
		 glm::vec3 V, L, R, N, Vt;
		 glm::vec3 auxDir = glm::vec3(ray.direction.x * 0.001, ray.direction.y * 0.001, ray.direction.z * 0.001); 
		 V = glm::normalize(Camera::getInstance()->computeV());
		 N = glm::normalize(closestNormal);

	//	 Compute the illumination
		glm::vec3 lightPoint;
		for(std::vector<Light>::iterator l = nff->lights.begin(); l != nff->lights.end(); l++) {
			lightPoint = glm::vec3(l->position.px, l->position.py, l->position.pz);
			L = glm::normalize(lightPoint - closestPi);
			
			//inicializacao do shadow feeler
			Ray shadowFeeler;
			shadowFeeler.origin = closestPi - auxDir;
			shadowFeeler.direction = L;
			
			//vector reflexao especular
			R = glm::normalize(2 * glm::dot(V, N) * N - V); 

			float LdotN = std::max(glm::dot(L, N), 0.0f); //para o calculo do material
			float attenuation;
			if(LdotN > 0) {		
				// se intersecta com um shadow feelerb
				if(intersecta(nff, shadowFeeler)){
					continue;
				}
				glm::vec3 lightPosition(l->position.px, l->position.py, l->position.pz);
				attenuation = 1.0f / (1.0f + lightAttenuation.x * glm::length(closestPi - lightPosition) + lightAttenuation.y * pow(glm::length(closestPi - lightPosition), 2.0f));
				// Componente difusa
				diffuseR += material.kd * material.color.r * l->color.r * LdotN;
				diffuseG += material.kd * material.color.g * l->color.g * LdotN;
				diffuseB += material.kd * material.color.b * l->color.b * LdotN;

				//componente especular
				float RdotL = std::max(glm::dot(R,L), 0.0f);
				if(RdotL > 0){ 
					specularR += material.ks * material.color.r * l->color.r * glm::pow(RdotL, material.shine) * attenuation;
					specularG += material.ks * material.color.g * l->color.g * glm::pow(RdotL, material.shine) * attenuation;
					specularB += material.ks * material.color.b * l->color.b * glm::pow(RdotL, material.shine) * attenuation;
				}
			}//end if(LdotN > 0)

		} //end for

		material.color.r = diffuseR + specularR;
		material.color.g = diffuseG + specularG;
		material.color.b = diffuseB + specularB;

		// Compute the secondary rays
		if(depth < MAX_DEPTH){
			// Calculo da reflexao
			
			if(material.ks > 0){
//				niu2 = material.indexRefraction;
				Ray reflectionRay = computeReflectionRay(closestPi, R);
				RGB reflectionColor = trace(nff, reflectionRay, depth + 1, ior);

				material.color.r += reflectionColor.r * material.ks;
				material.color.g += reflectionColor.g * material.ks;
				material.color.b += reflectionColor.b * material.ks;
			}
		
			// Refraction
			if(material.t != 0) {
				//Vt = glm::dot(V, N) * N - V;
				Vt = glm::dot(-ray.direction, N) * N + ray.direction;
				//float result;
				//if(ior != 1.0f){
				//	//niu1 = 1.0f;
				//	niu1 = ior;
				//	niu2 = material.indexRefraction;

				//} else {
				//	niu1 = material.indexRefraction;
				//	//niu2 = 1.0f;	
				//	niu2 = ior;
				//}
				//result = niu1;
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
	//std::cout << ((P1.x - P0.x) * (P2.y - P0.y) - (P2.x -  P0.x) * (P1.y - P0.y)) << std::endl;
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
				//	std::cout << "+1" << std::endl;
					++wn;     
				}
			}
		}
		else {                        
			if (v1->y  <= point.y) {
				if (isLeft( *v, *v1, point) < 0) { 
				//	std::cout << "-1" << std::endl;
					--wn;                  
				}
			}
		}
	}
	//std::cout << "wn: " << wn << std::endl;
	return wn;
}

bool RayTracer::polygonContainsPoint(	glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, 
										glm::vec3 normal, glm::vec3 point) {
	glm::vec2 projectedV1, projectedV2, projectedV3, projectedPoint;
	glm::vec3 absNormal = glm::vec3(std::abs(normal.x), std::abs(normal.y), std::abs(normal.z));
	float axisToIgnore = std::max(std::max(absNormal.x, absNormal.y), absNormal.z);
	// 2D Projection
	if(axisToIgnore == absNormal.x) {
		//std::cout << "axis x, plane yz " << std::endl;
		projectedV1 = glm::vec2(v1.y, v1.z);
		projectedV2 = glm::vec2(v2.y, v2.z);
		projectedV3 = glm::vec2(v3.y, v3.z);
		projectedPoint = glm::vec2(point.y, point.z);
	}
	else if(axisToIgnore == absNormal.y) {
		//std::cout << "axis y, plane xz " << std::endl;
		projectedV1 = glm::vec2(v1.x, v1.z);
		projectedV2 = glm::vec2(v2.x, v2.z);
		projectedV3 = glm::vec2(v3.x, v3.z);
		projectedPoint = glm::vec2(point.x, point.z);
	}
	else {
		//std::cout << "axis z, plane xy " << std::endl;
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

	/**Ti = t;
	*normal = glm::normalize(N);
	return true;
	*/
	// ver se o ponto de intersecao com o plano esta dentro do poligono
	if (polygonContainsPoint(v1, v2,v3, N, *Pi)) {
		*Ti = t;
		*normal = glm::normalize(N);
		return true;
	}
	return false; 
}


/* Calculo da intersecao de um raio a um plano 
 * (ponto A, B, C)
 * 1 - calcular a normal ao plano --> N = AB x AC = (B - A) x (C - A)
 * 2 - calcular prod interno da normal e direccao do ray
 *     Se o resultado for 0 => a recta e' paralela ao plano => nao intersecta
 * 3 - calcular o t = - (N.origin / N.direction) 
 */
bool RayTracer::intersect(glm::vec3 * Pi, float * Ti, glm::vec3 * normal, Plan plan, Ray ray){
	glm::vec3 A = glm::vec3(plan.point_1.px, plan.point_1.py, plan.point_1.pz);
	glm::vec3 B = glm::vec3(plan.point_2.px, plan.point_2.py, plan.point_2.pz);
	glm::vec3 C = glm::vec3(plan.point_3.px, plan.point_3.py, plan.point_3.pz);
	glm::vec3 N = glm::cross((B - A), (C - A));

	float NdotD = glm::dot(N, ray.direction);
	if(NdotD == 0){
		return false;
	}

	glm::vec3 sub = ray.origin - A;
	float NdotO = glm::dot(N, sub);
	float t = - (NdotO/ NdotD);
	if(t < 0){
		return false;
	}

	//if(NdotD < 0){
		//N = -N;
	//}

	// calcular o ponto de intersecao
	*Pi = ray.origin + ray.direction*t;
	*Ti = t;
	*normal = glm::normalize(N);
	return true;
}

bool RayTracer::intersect(glm::vec3 * Pi, float * Ti, glm::vec3 * normal, Polygon polygon, Ray ray){
	glm::vec3 v1 = glm::vec3(polygon.vertices[0].vx, polygon.vertices[0].vy, polygon.vertices[0].vz);
	glm::vec3 v2 = glm::vec3(polygon.vertices[1].vx, polygon.vertices[1].vy, polygon.vertices[1].vz);
	glm::vec3 v3 = glm::vec3(polygon.vertices[2].vx, polygon.vertices[2].vy, polygon.vertices[2].vz);

	return intersectPolygonAux(Pi, Ti, normal, ray, v1, v2, v3);
}

bool RayTracer::intersect(glm::vec3 * Pi, float * Ti, glm::vec3 * normal, PolygonPatch polygonPatch, Ray ray){
	glm::vec3 v1 = glm::vec3(polygonPatch.vertices[0].vx, polygonPatch.vertices[0].vy, polygonPatch.vertices[0].vz);
	glm::vec3 v2 = glm::vec3(polygonPatch.vertices[1].vx, polygonPatch.vertices[1].vy, polygonPatch.vertices[1].vz);
	glm::vec3 v3 = glm::vec3(polygonPatch.vertices[2].vx, polygonPatch.vertices[2].vy, polygonPatch.vertices[2].vz);

	return intersectPolygonAux(Pi, Ti, normal, ray, v1, v2, v3);
}

/* Calculo da intersecao de um raio a uma esfera 
 * 1 - normalizar vector direccao
 * 2 - calcular o quadrado da distancia da origem do raio ao centro da esfera
 * 3 - comparar o quadrado da distancia com o quadrado do raio da esfera, se for igual retorna falso
 * 4 - calcular o B
 */
bool RayTracer::intersect(glm::vec3 * Pi, float * Ti, glm::vec3 * normal, Sphere sphere, Ray ray){
	//passo 1
	glm::vec3 D = glm::normalize(glm::vec3(ray.direction[0], ray.direction[1], ray.direction[2]));
	glm::vec3 O = glm::vec3(ray.origin[0], ray.origin[1], ray.origin[2]);
	glm::vec3 C = glm::vec3(sphere.center.px, sphere.center.py, sphere.center.pz);
	//IsInside = false;

	//passo 2
	float dQuad = glm::pow2(C.x - O.x) + glm::pow2(C.y - O.y) + glm::pow2(C.z - O.z);

	//passo 3
	float radiusQuad = glm::pow2(sphere.radius);
	if(dQuad == radiusQuad){
		return false;
	}

	//passo 4
	float B = D.x * (C.x - O.x) + D.y * (C.y - O.y) + D.z * (C.z - O.z);
	
	//passo 5
	if(dQuad > radiusQuad){
		if( B < 0)
			return false;
	}

	//passo 6
	float R = glm::pow2(B) - dQuad + radiusQuad;
	if(R < 0){
		return false;
	}

	//passo 7
	if(dQuad > radiusQuad){
		*Ti = B - glm::sqrt(R);
	} 
	else if(dQuad < radiusQuad){
		*Ti = B + glm::sqrt(R);
	}

	//passo 8
	*Pi = O + D * *Ti;

	//passo 9
	float t = *Ti;
	glm::vec3 pi = *Pi;
	*normal = glm::normalize((pi - C)/sphere.radius);

	if(dQuad < radiusQuad){
		*normal = *normal * -1.0f;
	}

	return true;
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


Ray RayTracer::computeReflectionRay(glm::vec3 Pi, glm::vec3 r){
	Ray ray;
	ray.origin = Pi + (0.001f * r);
	ray.direction = r;
	return ray;
}

Ray RayTracer::computeRefractionRay(glm::vec3 Pi, glm::vec3 Vt, glm::vec3 N, float ior, float iorObject, float * newIOR){
	Ray ray;
	float sinThetaI, sinThetaT, cosThetaT;
//	float newIOR;

	if(ior != 1.0f) 
		*newIOR = 1.0f;
	else *newIOR = iorObject;

	//glm::vec3 Vt = glm::dot(V, N) * N - V;

	glm::vec3 t = glm::normalize(Vt); 
	glm::vec3 rt;

	//sinThetaI = glm::length(Vt);
	//sinThetaI = glm::sqrt(glm::length(Vt));
	sinThetaI = glm::sqrt(glm::pow2(Vt.x) + glm::pow2(Vt.y) + glm::pow2(Vt.z));
	sinThetaT = (ior/(*newIOR)) * sinThetaI;
	//sinThetaT = (niu2/niu1) * sinThetaI;
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