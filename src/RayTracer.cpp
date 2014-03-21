#include "RayTracer.h"

Color RayTracer::trace(Ray ray, int depth){
	return Color();
}

bool RayTracer::intersect(Sphere sphere, Ray ray){
	return false;
}

bool RayTracer::intersect(Plan plan, Ray ray){
	return false;
}

bool RayTracer::intersect(Polygon polygon, Ray ray){
	return false;
}

bool RayTracer::intersect(ConeCylinder coneCylinder, Ray ray){
	return false;
}

bool RayTracer::intersect(PolygonPatch polygonPatch, Ray ray){
	return false;
}

Ray RayTracer::reflectionRay(Ray ray){
	return ray; // nao e isto
}// ?!

Ray RayTracer::refractionRay(Ray ray){
	return ray; // nao e isto
}// ?!