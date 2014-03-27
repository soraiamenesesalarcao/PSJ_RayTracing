#pragma once

#include "Definitions.h"
#include "Camera.h"

class RayTracer {

	private:
		RayTracer();
		
	public:
		static RayTracer * getInstance();
		bool RayTracer::intersecta(NFF *nff, Ray ray);
		float isLeft(glm::vec2 P0, glm::vec2 P1, glm::vec2 P2);
		int windingNumber(std::vector<glm::vec2> vertices, glm::vec2 point);
		bool polygonContainsPoint(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 normal, glm::vec3 point);
		bool intersectPolygonAux(glm::vec3 * Pi, float * Ti, glm::vec3 * normal, Ray ray, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);		
		bool intersect(glm::vec3 * Pi, float * Ti, glm::vec3 * normal, Plan plan, Ray ray);
		bool intersect(glm::vec3 * Pi, float * Ti, glm::vec3 * normal, Polygon polygon, Ray ray);
		bool intersect(glm::vec3 * Pi, float * Ti, glm::vec3 * normal, PolygonPatch polygonPatch, Ray ray);
		bool intersect(glm::vec3 * Pi, float * Ti, glm::vec3 * normal, Sphere sphere, Ray ray);
		bool intersect(glm::vec3 * Pi, float * Ti, glm::vec3 * normal, ConeCylinder coneCylinder, Ray ray);
		
		Ray computeReflectionRay(glm::vec3 Pi, glm::vec3 r); 
		Ray computeRefractionRay(glm::vec3 Pi, glm::vec3 Vt, glm::vec3 N, float niu1, float niu2);

		RGB trace(NFF * nff, Ray ray, int depth, float ior);
};