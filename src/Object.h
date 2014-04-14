#pragma once

#include "BoundingBox.h"
#include "Material.h"
#include "Ray.h"

class Object{

	protected:
		Material _mtl;
		BoundingBox _bb;
		int _lastRayID;

	public:
		Object(Material mtl);
		Material getMaterial();
		BoundingBox getBoundingBox();
		int getLastRayID();
		void setLastRayID(int ID);
		virtual void setBoundingBox() = 0;

		virtual bool intersect(glm::vec3 * Pi, float * Ti, glm::vec3 * normal, Ray ray) = 0;

};