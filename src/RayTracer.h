#pragma once

#include "Definitions.h"
#include "Camera.h"
#include "Light.h"
#include "Object.h"
#include "Sphere.h"
#include "Polygon.h"
#include "Ray.h"
#include "Grid.h"

class RayTracer {

	private:
		Grid _grid;
		bool _usingGrid;
		int _nRays;
	public:
		RayTracer();
		~RayTracer();
		int getNRays();
		void incNRays();
		void clearNRays();
		void init(std::vector<Object*> objects);
		RGB trace(RGB * background, std::vector<Light> lights, std::vector<Object*> objects, 
					Ray * ray, int depth, float ior, glm::vec3 V);
		Object * cellTraversal(	Cell * startingCell, glm::vec3 rayDelta, glm::vec3 rayMax, Ray * ray);
		Object* closestIntersection(std::vector<Object*> objects, Ray * ray);
		void toggleUsingGrid();
};