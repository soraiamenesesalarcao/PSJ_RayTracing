#pragma once

#include "Color.h"
#include "Definitions.h"

class RayTracer {

	private:
		
	public:
		bool intersect(Sphere sphere, Ray ray);
		bool intersect(Plan plan, Ray ray);
		bool intersect(Polygon polygon, Ray ray);
		bool intersect(ConeCylinder coneCylinder, Ray ray);
		bool intersect(PolygonPatch polygonPatch, Ray ray);
		Ray reflectionRay(Ray ray); // ?!
		Ray refractionRay(Ray ray); // ?!
		Color trace(Ray ray, int depth);
};