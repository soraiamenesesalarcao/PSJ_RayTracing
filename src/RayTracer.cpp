#include "RayTracer.h"

RayTracer::RayTracer() {
}

RayTracer * RayTracer::getInstance(){
	static RayTracer instance;
	return &instance;
}

bool RayTracer::intersecta(NFF *nff, Ray ray){
	bool hasIntersectedGlobal = false;
	bool hasIntersectedLocal = false;

	glm::vec3 Pi;
	glm::vec3 normal;
	float Ti;
	bool shadow = false;

	for(std::vector<Plan>::iterator pl = nff->planes.begin(); pl != nff->planes.end(); pl++) {
		hasIntersectedLocal = intersect(&Pi, &Ti, &normal, *pl, ray);
		//if(hasIntersectedLocal) {
		if(Ti <= 1 && Ti >= 0){
			return true;
		}
	}

	for(std::vector<Sphere>::iterator s = nff->spheres.begin(); s != nff->spheres.end(); s++) {
		hasIntersectedLocal = intersect(&Pi, &Ti, &normal, *s, ray);
		//if(hasIntersectedLocal) {
		if(Ti <= 1 && Ti >= 0){
			return true;
		}
	}

	return false;
}

RGB RayTracer::trace(NFF * nff, Ray ray, int depth){
	bool hasIntersectedGlobal = false;
	bool hasIntersectedLocal = false;
	glm::vec3 Pi;
	glm::vec3 closestPi;
	glm::vec3 normal;
	glm::vec3 closestNormal;
	float Ti;
	float closestTi = FLT_MAX;

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
		 glm::vec3 V, L, H, R, N;
		 glm::vec3 auxDir = glm::vec3(ray.direction.x * 0.001, ray.direction.y * 0.001, ray.direction.z * 0.001); 
		 V = glm::normalize(Camera::getInstance()->computeV());
		 N = glm::normalize(closestNormal);

	//	 Compute the illumination
		glm::vec3 lightPoint;
		for(std::vector<Light>::iterator l = nff->lights.begin(); l != nff->lights.end(); l++) {
			lightPoint = glm::vec3(l->position.px, l->position.py, l->position.pz);
			L = glm::normalize(lightPoint - closestPi); // S // - closestPi);
			
			//SHADOWS FEELEERS
			Ray shadowFeeleer;
			
			shadowFeeleer.origin = closestPi - auxDir;
			shadowFeeleer.direction = L;
			
			float NdotL = std::max(glm::dot(N, L), 0.0f); //para o calculo do material

			if(NdotL > 0) {
				// se intersecta com um shadow feeleer
				if(intersecta(nff, shadowFeeleer)){
					continue;
				}
								
				H = glm::normalize(-V + L);
				float NdotH = std::max(glm::dot(N, H), 0.0f);  //calculo da especular

				diffuseR += material.color.r * l->color.r * NdotL;
				diffuseG += material.color.g * l->color.g * NdotL;
				diffuseR += material.color.b * l->color.b * NdotL;

				if(NdotH > 0){ 
					specularB += material.color.r * l->color.r * glm::pow(NdotH, material.shine);
					specularG += material.color.g * l->color.g * glm::pow(NdotH, material.shine);
					specularR += material.color.b * l->color.b * glm::pow(NdotH, material.shine);
				}
			}
			glm::vec3 ambient = glm::vec3(0.2, 0.2, 0.2);
			material.color.r = std::min((ambient.x + material.kd * diffuseR + material.ks * specularR), 1.0f);
			material.color.g =  std::min((ambient.y + material.kd * diffuseG + material.ks * specularG), 1.0f);
			material.color.b =  std::min((ambient.z + material.kd * diffuseB + material.ks * specularB), 1.0f);
		}

		//Compute the secondary rays
		
		if(depth <= MAX_DEPTH) {
			glm::vec3 D = glm::vec3(ray.direction);
			R = D - 2 * glm::dot(D, N) * N;

			// Reflection
			/**/
			if(material.ks > 0) { 
				Ray reflectionRay = computeReflectionRay(closestPi, R);
				RGB reflectionColor = trace(nff, reflectionRay, depth + 1);
				material.color.r = std::min((material.color.r + reflectionColor.r * material.ks), 1.0f);
				material.color.g = std::min((material.color.g + reflectionColor.g * material.ks), 1.0f);
				material.color.b = std::min((material.color.b + reflectionColor.b * material.ks), 1.0f);
			}
			/**/
			// Refraction
			/*if(material.t > 0) {
				Ray refractionRay = computeRefractionRay(ray);
				RGB refractionColor = trace(nff, ray, depth + 1);
				material.color.r += (refractionColor.r / material.t);
				material.color.g += (refractionColor.g / material.t);
				material.color.b += (refractionColor.b / material.t);
			}*/
		} 
	}

	return material.color;
}


bool RayTracer::intersectPolygonAux(glm::vec3 * Pi, float * Ti, glm::vec3 * normal, Ray ray, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3){
	glm::vec3 N = glm::cross((v3 - v2), (v1 - v2));
	glm::vec3 Q = v1;

	float t = (glm::dot(N, (Q - ray.origin)))/(glm::dot(ray.origin, ray.direction));

	if (t < 0)
		return false;	
	else {
		*Pi = ray.origin + ray.direction*t;
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


bool RayTracer::intersect(glm::vec3 * Pi, float * Ti, glm::vec3 * normal, Sphere sphere, Ray ray){
	glm::vec3 D = glm::vec3(ray.direction[0], ray.direction[1], ray.direction[2]);
	glm::vec3 O = glm::vec3(ray.origin[0], ray.origin[1], ray.origin[2]);
	glm::vec3 C = glm::vec3(sphere.center.px, sphere.center.py, sphere.center.pz);

	float a = glm::dot(D, D);
	float b = 2 * D.x * (O.x - C.x) 
			+ 2 * D.y * (O.y - C.y) 
			+ 2 * D.z * (O.z - C.z);
	float c = glm::dot(C, C) + glm::dot(O, O) - 2 * glm::dot(C, O) - glm::pow2(sphere.radius);

	float disc = glm::pow2(b) - 4*a*c;

	if (disc < 0)
		return false;
	else{
		float t = (-b - glm::sqrt(disc))/(2*a);
		// calcular o ponto de intersecao mais proximo
		*Pi = ray.origin + ray.direction*t;
		*Ti = t;
		*normal = glm::normalize(glm::vec3((Pi->x - sphere.center.px)/sphere.radius, 
							(Pi->y - sphere.center.py)/sphere.radius, 
							(Pi->z - sphere.center.pz)/sphere.radius));
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


Ray RayTracer::computeReflectionRay(glm::vec3 Pi, glm::vec3 r){
	Ray ray;
	ray.origin = Pi + (0.001f * r);
	ray.direction = r;
	return ray;
}

Ray RayTracer::computeRefractionRay(Ray ray){
	return ray; // TO DO
}

void RayTracer::castShadowFeeler(NFF * nff, Ray ray, glm::vec3 Pi) {

}