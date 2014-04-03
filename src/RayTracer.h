#pragma once

#include "Definitions.h"
#include "Camera.h"
#include "Light.h"
#include "Object.h"
#include "Ray.h"

class RayTracer {

	private:
		RayTracer();
		
	public:
		static RayTracer * getInstance();	
		RGB trace(RGB * background, std::vector<Light> lights, std::vector<Object*> objects, Ray ray, int depth, float ior);
		Object* closestIntersection(std::vector<Object*> objects, glm::vec3 &Pi, glm::vec3 &normal, Ray ray);
};