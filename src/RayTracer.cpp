#include "RayTracer.h"

Color RayTracer::trace(Ray ray, int depth){
	return Color();
}

bool RayTracer::intersect(Sphere sphere, Ray ray){
	return false;
}

/* Calculo da intersecao de um raio a um plano 
 * (ponto A, B, C)
 * 1 - calcular a normal ao plano --> N = AB x AC = (B - A) x (C - A)
 * 2 - calcular prod interno da normal e direccao do ray
 *     Se o resultado for 0 => a recta � paralela ao plano => n�o intersecta
 * 3 - calcular o t = - (N.origin / N.direction) 
 */
bool RayTracer::intersect(Plan plan, Ray ray){
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
	Pi = ray.origin + ray.direction*t;
	return true;
}

glm::vec3 RayTracer::getNormal(){
	return glm::normalize(normal);
}

bool RayTracer::intersect(Polygon polygon, Ray ray){
	return false;
}

bool RayTracer::intersect(ConeCylinder coneCylinder, Ray ray){
	return false;
}

bool RayTracer::intersect(PolygonPatch polygonPatch, Ray ray){
	return false;
}

Ray RayTracer::reflectionRay(Ray ray){
	return ray; // nao e isto
}// ?!

Ray RayTracer::refractionRay(Ray ray){
	return ray; // nao e isto
}// ?!