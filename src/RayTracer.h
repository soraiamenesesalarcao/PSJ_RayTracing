#pragma once

#include "Color.h"
#include "Definitions.h"

class RayTracer {

	private:
	//	glm::vec3 normal;
	//	glm::vec3 Pi; //Point intersection
		
	public:
		bool intersectPolygonAux(glm::vec3 * Pi, glm::vec3 * normal, Ray ray, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);		
		bool intersect(glm::vec3 * Pi, glm::vec3 * normal, Plan plan, Ray ray);
		bool intersect(glm::vec3 * Pi, glm::vec3 * normal, Polygon polygon, Ray ray);
		bool intersect(glm::vec3 * Pi, glm::vec3 * normal, PolygonPatch polygonPatch, Ray ray);
		bool intersect(glm::vec3 * Pi, glm::vec3 * normal, Sphere sphere, Ray ray);
		bool intersect(glm::vec3 * Pi, glm::vec3 * normal, ConeCylinder coneCylinder, Ray ray);
		
		Ray computeReflectionRay(Ray ray); // ?!
		Ray computeRefractionRay(Ray ray); // ?!
		RGB trace(NFF * nff, Ray ray, int depth);
	//	glm::vec3 getNormal();
};