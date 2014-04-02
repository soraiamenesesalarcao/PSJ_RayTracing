#pragma once

#include "Material.h"

class Object{

	protected:
		Material _mtl;

	public:
		Object(Material mtl);
		Material getMaterial();

		virtual bool intersect(glm::vec3 * Pi, float * Ti, glm::vec3 * normal, Ray ray) = 0;

};