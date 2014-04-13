#pragma once

#include "Definitions.h"
#include "Ray.h"

class BoundingBox {

	private:
		glm::vec3 _posMin;
		glm::vec3 _posMax;
		
	public:
		BoundingBox();
		glm::vec3 getPosMin();
		glm::vec3 getPosMax();
		void setPosMin(float x, float y, float z);
		void setPosMax(float x, float y, float z);
		bool intersect(Ray ray);

		
};