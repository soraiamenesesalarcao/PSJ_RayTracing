#pragma once

#include "Color.h"
#include "Definitions.h"

class RayTracer {

	private:
		glm::vec3 normal;
		glm::vec3 Pi; //Point intersection
		
	public:
		bool intersectPolygonAux(Ray ray, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);
		bool intersect(Sphere sphere, Ray ray);
		bool intersect(Plan plan, Ray ray);
		bool intersect(Polygon polygon, Ray ray);
		bool intersect(ConeCylinder coneCylinder, Ray ray);
		bool intersect(PolygonPatch polygonPatch, Ray ray);
		Ray reflectionRay(Ray ray); // ?!
		Ray refractionRay(Ray ray); // ?!
		Color trace(Ray ray, int depth);
		glm::vec3 getNormal();
};