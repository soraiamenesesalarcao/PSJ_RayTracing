#include "RayTracer.h"

RGB RayTracer::trace(NFF * nff, int pointX, int pointY, Ray ray, int depth){
	bool hasIntersectedGlobal = false;
	bool hasIntersectedLocal = false;
	glm::vec3 Pi;
	glm::vec3 closestPi;

	// Default material
	Material material;
	material.color = nff->background;
	material.indexRefraction = 1.0;
	material.kd = 0.0;
	material.ks = 0.0;
	material.shine = 0.0;
	material.t = 0.0;

	// Check if there is an intersection
	for(std::vector<Sphere>::iterator s = nff->spheres.begin(); s != nff->spheres.end(); s++) {
		hasIntersectedLocal = intersect(&Pi, *s, ray);
		if(hasIntersectedLocal) {
			if(glm::length(Pi - closestPi) < 0) { // distancia euclideana
				closestPi = Pi;
				material = s->mtl;
			}
			if(!hasIntersectedGlobal) {
				hasIntersectedGlobal = true;
				closestPi = Pi;
			}
		}
	}
	for(std::vector<Polygon>::iterator p = nff->polygons.begin(); p != nff->polygons.end(); p++) {
		hasIntersectedLocal = intersect(&Pi, *p, ray);
		if(hasIntersectedLocal) {
			if(glm::length(Pi - closestPi) < 0) { // distancia euclideana
				closestPi = Pi;
				material = p->mtl;
			}
			if(!hasIntersectedGlobal) {
				hasIntersectedGlobal = true;
				closestPi = Pi;
			}
		}
	}
	for(std::vector<PolygonPatch>::iterator pp = nff->polygonPatchs.begin(); pp != nff->polygonPatchs.end(); pp++) {
		hasIntersectedLocal = intersect(&Pi, *pp, ray);
		if(hasIntersectedLocal) {
			if(glm::length(Pi - closestPi) < 0) { // distancia euclideana
				closestPi = Pi;
				material = pp->mtl;
			}
			if(!hasIntersectedGlobal) {
				hasIntersectedGlobal = true;
				closestPi = Pi;
			}
		}

	}
	for(std::vector<ConeCylinder>::iterator c = nff->coneAndCylinders.begin(); c != nff->coneAndCylinders.end(); c++) {
		hasIntersectedLocal = intersect(&Pi, *c, ray);
		if(hasIntersectedLocal) {
			if(glm::length(Pi - closestPi) < 0) { // distancia euclideana
				closestPi = Pi;
				material = c->mtl;
			}
			if(!hasIntersectedGlobal) {
				hasIntersectedGlobal = true;
				closestPi = Pi;
			}
		}
	}
	for(std::vector<Plan>::iterator pl = nff->planes.begin(); pl != nff->planes.end(); pl++) {
		hasIntersectedLocal = intersect(&Pi, *pl, ray);
		if(hasIntersectedLocal) {
			if(glm::length(Pi - closestPi) < 0) { // distancia euclideana
				closestPi = Pi;
				material = pl->mtl;
			}
			if(!hasIntersectedGlobal) {
				hasIntersectedGlobal = true;
				closestPi = Pi;
			}
		}
	}

	if(hasIntersectedGlobal) {
		// Compute the illumination
		//glm::vec3 currentPoint = glm::vec3(pointX, pointY, 0.0);
		glm::vec3 lightPoint;
		for(std::vector<Light>::iterator l = nff->lights.begin(); l != nff->lights.end(); l++) {
			lightPoint = glm::vec3(l->position.px, l->position.py, l->position.pz);
			//glm::vec3 L = glm::normalize(lightPoint - currentPoint); // tudo errado :P			
		}


		//Compute the secondary rays
		if(depth <= MAX_DEPTH) {
			// Reflection
			if(material.kd > 0 || material.ks > 0) { 
				Ray reflectionRay = computeReflectionRay(ray);
				RGB reflectionColor = trace(nff, pointX, pointY, ray, depth + 1);
			}

			// Refraction
			if(material.t > 0) {
				Ray refractionRay = computeRefractionRay(ray);
				RGB refractionColor = trace(nff, pointX, pointY, ray, depth + 1);
			}
		}
	}

	return material.color;
}

glm::vec3 RayTracer::getNormal(){
	return glm::normalize(normal);
}


bool RayTracer::intersectPolygonAux(glm::vec3 * Pi, Ray ray, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3){
	glm::vec3 N = glm::cross((v3 - v2), (v1 - v2));
	glm::vec3 Q = v1;

	float t = (glm::dot(N, (Q - ray.origin)))/(glm::dot(ray.origin, ray.direction));

	if (t < 0)
		return false;	
	else {
		*Pi = ray.origin + ray.direction*t;
		return true;
	}
	return false;
}

bool RayTracer::intersect(glm::vec3 * Pi, Sphere sphere, Ray ray){
	glm::vec3 D = glm::vec3(ray.direction[0], ray.direction[1], ray.direction[2]);
	glm::vec3 O = glm::vec3(ray.origin[0], ray.origin[1], ray.origin[2]);

	float A = glm::dot(D, D);
	float B = 2 * glm::dot(D, O);
	float C = glm::dot(O, O) - glm::pow2(sphere.radius);

	float disc = glm::pow2(B) - 4*A*C;

	if (disc < 0)
		return false;
	else{
		float t = (-B - glm::sqrt(disc))/(2*A);
		// calcular o ponto de intersecao mais próximo
		*Pi = ray.origin + ray.direction*t;
		return true;
	}
	return false;
}

/* Calculo da intersecao de um raio a um plano 
 * (ponto A, B, C)
 * 1 - calcular a normal ao plano --> N = AB x AC = (B - A) x (C - A)
 * 2 - calcular prod interno da normal e direccao do ray
 *     Se o resultado for 0 => a recta é paralela ao plano => não intersecta
 * 3 - calcular o t = - (N.origin / N.direction) 
 */
bool RayTracer::intersect(glm::vec3 * Pi, Plan plan, Ray ray){
	glm::vec3 A = glm::vec3(plan.point_1.px, plan.point_1.py, plan.point_1.pz);
	glm::vec3 B = glm::vec3(plan.point_2.px, plan.point_2.py, plan.point_2.pz);
	glm::vec3 C = glm::vec3(plan.point_3.px, plan.point_3.py, plan.point_3.pz);

	normal = glm::cross((B - A), (C - A));

	float NdotD = glm::dot(normal, ray.direction);
	if(NdotD == 0){
		return false;
	}

	float NdotO = glm::dot(normal, ray.origin);
	float t = - (NdotO/ NdotD);
	if(t < 0){
		return false;
	}

	if(NdotD < 0){
		normal = -normal;
	}
	// calcular o ponto de intersecao
	*Pi = ray.origin + ray.direction*t;
	return true;
}

bool RayTracer::intersect(glm::vec3 * Pi, Polygon polygon, Ray ray){
	glm::vec3 v1 = glm::vec3(polygon.vertices[0].vx, polygon.vertices[0].vy, polygon.vertices[0].vz);
	glm::vec3 v2 = glm::vec3(polygon.vertices[1].vx, polygon.vertices[1].vy, polygon.vertices[1].vz);
	glm::vec3 v3 = glm::vec3(polygon.vertices[2].vx, polygon.vertices[2].vy, polygon.vertices[2].vz);

	return intersectPolygonAux(Pi, ray, v1, v2, v3);
}

bool RayTracer::intersect(glm::vec3 * Pi, ConeCylinder coneCylinder, Ray ray){
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
		if(z1 < z2)
			*Pi = ray.origin + t1 * ray.direction;
		else *Pi = ray.origin + t2 * ray.direction;
		return true;
	}
	else return false;
	return false;
}

bool RayTracer::intersect(glm::vec3 * Pi, PolygonPatch polygonPatch, Ray ray){
	glm::vec3 v1 = glm::vec3(polygonPatch.vertices[0].vx, polygonPatch.vertices[0].vy, polygonPatch.vertices[0].vz);
	glm::vec3 v2 = glm::vec3(polygonPatch.vertices[1].vx, polygonPatch.vertices[1].vy, polygonPatch.vertices[1].vz);
	glm::vec3 v3 = glm::vec3(polygonPatch.vertices[2].vx, polygonPatch.vertices[2].vy, polygonPatch.vertices[2].vz);

	return intersectPolygonAux(Pi, ray, v1, v2, v3);
}

Ray RayTracer::computeReflectionRay(Ray ray){
	return ray; // nao e isto
}// ?!

Ray RayTracer::computeRefractionRay(Ray ray){
	return ray; // nao e isto
}// ?!