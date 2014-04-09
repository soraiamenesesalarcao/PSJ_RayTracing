#pragma once

#include "Object.h"

class Sphere : public Object{

	private:
		glm::vec3 _center;
		float _radius;

	public:
		Sphere(glm::vec3 center, float radius, Material mtl);
		glm::vec3 getPoint(){ return _center; }
		float getRadius(){ return _radius; } 
		void setBoundingBox();
		bool intersect(glm::vec3 * Pi, float * Ti, glm::vec3 * normal, Ray ray);
};