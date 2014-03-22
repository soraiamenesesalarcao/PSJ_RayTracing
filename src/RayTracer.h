#pragma once

#include "Color.h"
#include "Definitions.h"

class RayTracer {

	private:
		glm::vec3 normal;
	//	glm::vec3 Pi; //Point intersection
		
	public:
		bool intersectPolygonAux(glm::vec3 * Pi, Ray ray, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);
		bool intersect(glm::vec3 * Pi, Sphere sphere, Ray ray);
		bool intersect(glm::vec3 * Pi, Plan plan, Ray ray);
		bool intersect(glm::vec3 * Pi, Polygon polygon, Ray ray);
		bool intersect(glm::vec3 * Pi, ConeCylinder coneCylinder, Ray ray);
		bool intersect(glm::vec3 * Pi, PolygonPatch polygonPatch, Ray ray);
		Ray reflectionRay(Ray ray); // ?!
		Ray refractionRay(Ray ray); // ?!
		Color trace(NFF * scene, Ray ray, int depth);
		glm::vec3 getNormal();
};