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
		bool _usingDoF;
		bool _usingSoftShadows;
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
		void toggleUsingSoftShadows();
		void setUsingDoF(bool dof);

		RGB monteCarlo(Camera camera, RGB * background, std::vector<Light> lights, std::vector<Object*> objects, float x, float y, int depth);
		RGB depthOfField(RGB * background, std::vector<Light> lights, std::vector<Object*> objects, Ray * ray, int depth, float ior, glm::vec3 V, Camera camera);
};