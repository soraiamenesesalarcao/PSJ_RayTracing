#pragma once

#include "Definitions.h"
#include "Camera.h"

class RayTracer {

	private:
		RayTracer();
		
	public:
		static RayTracer * getInstance();
		bool RayTracer::intersecta(NFF *nff, Ray ray);
		bool intersectPolygonAux(glm::vec3 * Pi, float * Ti, glm::vec3 * normal, Ray ray, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);		
		bool intersect(glm::vec3 * Pi, float * Ti, glm::vec3 * normal, Plan plan, Ray ray);
		bool intersect(glm::vec3 * Pi, float * Ti, glm::vec3 * normal, Polygon polygon, Ray ray);
		bool intersect(glm::vec3 * Pi, float * Ti, glm::vec3 * normal, PolygonPatch polygonPatch, Ray ray);
		bool intersect(glm::vec3 * Pi, float * Ti, glm::vec3 * normal, Sphere sphere, Ray ray);
		bool intersect(glm::vec3 * Pi, float * Ti, glm::vec3 * normal, ConeCylinder coneCylinder, Ray ray);
		
		Ray computeReflectionRay(glm::vec3 Pi, glm::vec3 r); 
		Ray computeRefractionRay(Ray ray); 
		void castShadowFeeler(NFF * nff, Ray ray, glm::vec3 Pi);
		RGB trace(NFF * nff, Ray ray, int depth);
};