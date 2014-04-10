#pragma once

#include "Definitions.h"
#include "Camera.h"
#include "Light.h"
#include "Object.h"
#include "Ray.h"

class RayTracer {

	private:
		
	public:
		RayTracer();
		~RayTracer();
		RGB trace(RGB * background, std::vector<Light> lights, std::vector<Object*> objects, Ray ray, int depth, float ior, glm::vec3 V);
		Object* closestIntersection(std::vector<Object*> objects, glm::vec3 &Pi, glm::vec3 &normal, Ray ray);
};