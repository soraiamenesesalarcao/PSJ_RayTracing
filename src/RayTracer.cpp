#include "RayTracer.h"

RGB RayTracer::trace(NFF * nff, Ray ray, int depth){
	bool hasIntersectedGlobal = false;
	bool hasIntersectedLocal = false;
	glm::vec3 Pi;
	glm::vec3 closestPi;
	glm::vec3 normal;
	glm::vec3 closestNormal;

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
		hasIntersectedLocal = intersect(&Pi, &normal, *pl, ray);
		if(hasIntersectedLocal) {
			if(glm::length(Pi - closestPi) < 0) { // distancia euclideana
				closestPi = Pi;
				closestNormal = normal;
				material = pl->mtl;
			}
			if(!hasIntersectedGlobal) {
				hasIntersectedGlobal = true;
			}
		}
	}
	for(std::vector<Polygon>::iterator p = nff->polygons.begin(); p != nff->polygons.end(); p++) {
		hasIntersectedLocal = intersect(&Pi, &normal, *p, ray);
		if(hasIntersectedLocal) {
			if(glm::length(Pi - closestPi) < 0) { // distancia euclideana
				closestPi = Pi;
				closestNormal = normal;
				material = p->mtl;
			}
			if(!hasIntersectedGlobal) {
				hasIntersectedGlobal = true;
			}
		}
	}
	for(std::vector<PolygonPatch>::iterator pp = nff->polygonPatchs.begin(); pp != nff->polygonPatchs.end(); pp++) {
		hasIntersectedLocal = intersect(&Pi, &normal, *pp, ray);
		if(hasIntersectedLocal) {
			if(glm::length(Pi - closestPi) < 0) { // distancia euclideana
				closestPi = Pi;
				closestNormal = normal;
				material = pp->mtl;
			}
			if(!hasIntersectedGlobal) {
				hasIntersectedGlobal = true;
			}
		}

	}
	for(std::vector<Sphere>::iterator s = nff->spheres.begin(); s != nff->spheres.end(); s++) {
		hasIntersectedLocal = intersect(&Pi, &normal, *s, ray);
		if(hasIntersectedLocal) {
			if(glm::length(Pi - closestPi) < 0) { // distancia euclideana
				closestPi = Pi;
				closestNormal = normal;
				material = s->mtl;
			}
			if(!hasIntersectedGlobal) {
				hasIntersectedGlobal = true;
			}
		}
	}
	for(std::vector<ConeCylinder>::iterator c = nff->coneAndCylinders.begin(); c != nff->coneAndCylinders.end(); c++) {
		hasIntersectedLocal = intersect(&Pi, &normal, *c, ray);
		if(hasIntersectedLocal) {
			if(glm::length(Pi - closestPi) < 0) { // distancia euclideana
				closestPi = Pi;
				closestNormal = normal;
				material = c->mtl;
			}
			if(!hasIntersectedGlobal) {
				hasIntersectedGlobal = true;
			}
		}
	}
	// Check ih there was an intersection
	if(hasIntersectedGlobal) {
		// Compute the illumination
		glm::vec3 lightPoint;
		for(std::vector<Light>::iterator l = nff->lights.begin(); l != nff->lights.end(); l++) {
			lightPoint = glm::vec3(l->position.px, l->position.py, l->position.pz);
			glm::vec3 L = glm::normalize(lightPoint - closestPi);
			glm::vec3 V = Camera::getInstance()->computeV();
			glm::vec3 H = glm::normalize(L + V);
			float NdotL = std::max(glm::dot(closestNormal, L), 0.0f);
			float NdotH = std::max(glm::dot(closestNormal, H), 0.0f);
			material.color.r = (material.color.r * l->color.r) * 
								(material.kd * NdotL //diffuse
								+ material.ks * glm::pow(NdotH, material.shine)); // specular
			material.color.g = material.kd * l->color.g * NdotL;
			material.color.b = material.kd * l->color.b * NdotL;
		}

		//Compute the secondary rays
		if(depth <= MAX_DEPTH) {
			// Reflection
			if(material.kd > 0 || material.ks > 0) { 
				Ray reflectionRay = computeReflectionRay(ray);
				RGB reflectionColor = trace(nff, ray, depth + 1);
				material.color.r += (reflectionColor.r / material.ks);
				material.color.g += (reflectionColor.g / material.ks);
				material.color.b += (reflectionColor.b / material.ks);
			}

			// Refraction
			if(material.t > 0) {
				Ray refractionRay = computeRefractionRay(ray);
				RGB refractionColor = trace(nff, ray, depth + 1);
				material.color.r += (refractionColor.r / material.t);
				material.color.g += (refractionColor.g / material.t);
				material.color.b += (refractionColor.b / material.t);
			}
		}
	}

	return material.color;
}

//glm::vec3 RayTracer::getNormal(){
//	return glm::normalize(normal);
//}

bool RayTracer::intersectPolygonAux(glm::vec3 * Pi, glm::vec3 * normal, Ray ray, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3){
	glm::vec3 N = glm::cross((v3 - v2), (v1 - v2));
	glm::vec3 Q = v1;

	float t = (glm::dot(N, (Q - ray.origin)))/(glm::dot(ray.origin, ray.direction));

	if (t < 0)
		return false;	
	else {
		*Pi = ray.origin + ray.direction*t;
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
bool RayTracer::intersect(glm::vec3 * Pi, glm::vec3 * normal, Plan plan, Ray ray){
	glm::vec3 A = glm::vec3(plan.point_1.px, plan.point_1.py, plan.point_1.pz);
	glm::vec3 B = glm::vec3(plan.point_2.px, plan.point_2.py, plan.point_2.pz);
	glm::vec3 C = glm::vec3(plan.point_3.px, plan.point_3.py, plan.point_3.pz);
	glm::vec3 N = glm::cross((B - A), (C - A));
	//*normal = glm::cross((B - A), (C - A));

	//float NdotD = glm::dot(*normal, ray.direction);
	float NdotD = glm::dot(N, ray.direction);
	if(NdotD == 0){
		return false;
	}

	//float NdotO = glm::dot(*normal, ray.origin);
	float NdotO = glm::dot(N, ray.origin);
	float t = - (NdotO/ NdotD);
	if(t < 0){
		return false;
	}

	if(NdotD < 0){
		//*normal = -(*normal);
		N = -N;
	}
	// calcular o ponto de intersecao
	*Pi = ray.origin + ray.direction*t;
	*normal = glm::normalize(N);
	return true;
}

bool RayTracer::intersect(glm::vec3 * Pi, glm::vec3 * normal, Polygon polygon, Ray ray){
	glm::vec3 v1 = glm::vec3(polygon.vertices[0].vx, polygon.vertices[0].vy, polygon.vertices[0].vz);
	glm::vec3 v2 = glm::vec3(polygon.vertices[1].vx, polygon.vertices[1].vy, polygon.vertices[1].vz);
	glm::vec3 v3 = glm::vec3(polygon.vertices[2].vx, polygon.vertices[2].vy, polygon.vertices[2].vz);

	return intersectPolygonAux(Pi, normal, ray, v1, v2, v3);
}

bool RayTracer::intersect(glm::vec3 * Pi, glm::vec3 * normal, PolygonPatch polygonPatch, Ray ray){
	glm::vec3 v1 = glm::vec3(polygonPatch.vertices[0].vx, polygonPatch.vertices[0].vy, polygonPatch.vertices[0].vz);
	glm::vec3 v2 = glm::vec3(polygonPatch.vertices[1].vx, polygonPatch.vertices[1].vy, polygonPatch.vertices[1].vz);
	glm::vec3 v3 = glm::vec3(polygonPatch.vertices[2].vx, polygonPatch.vertices[2].vy, polygonPatch.vertices[2].vz);

	return intersectPolygonAux(Pi, normal, ray, v1, v2, v3);
}


bool RayTracer::intersect(glm::vec3 * Pi, glm::vec3 * normal, Sphere sphere, Ray ray){
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
		// calcular o ponto de intersecao mais proximo
		*Pi = ray.origin + ray.direction*t;
		*normal = glm::normalize(glm::vec3((Pi->x - sphere.center.px)/sphere.radius, 
							(Pi->y - sphere.center.py)/sphere.radius, 
							(Pi->z - sphere.center.pz)/sphere.radius));
		return true;
	}
	return false;
}


bool RayTracer::intersect(glm::vec3 * Pi, glm::vec3 * normal, ConeCylinder coneCylinder, Ray ray){
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
		if(z1 < z2) // TO DO: calculate normals
			*Pi = ray.origin + t1 * ray.direction;
		else *Pi = ray.origin + t2 * ray.direction;
		return true;
	}
	else return false;
	return false;
}


Ray RayTracer::computeReflectionRay(Ray ray){
	return ray; // TO DO
}

Ray RayTracer::computeRefractionRay(Ray ray){
	return ray; // TO DO
}