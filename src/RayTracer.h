#pragma once

#include "Definitions.h"
#include "Camera.h"
#include "Light.h"
#include "Object.h"
#include "Ray.h"
#include "Grid.h"

class RayTracer {

	private:
		Grid _grid;
		bool _usingGrid;
	public:
		RayTracer();
		~RayTracer();
		RGB trace(RGB * background, std::vector<Light> lights, std::vector<Object*> objects, Ray ray, int depth, float ior, glm::vec3 V);
		Object* closestIntersection(std::vector<Object*> objects, glm::vec3 &Pi, glm::vec3 &normal, Ray ray);
		Object* closestIntersectionGrid(std::vector<Object*> objects, glm::vec3 &Pi, glm::vec3 &normal, Ray ray);
		void computeBoundingBoxes(std::vector<Object*> objects);
		void addObjectsToGrid(std::vector<Object*> objects);
		void toggleUsingGrid();
};