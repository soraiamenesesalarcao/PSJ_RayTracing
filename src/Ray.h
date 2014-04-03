#pragma once

#include "Definitions.h" 

class Ray {

	private:
		glm::vec3 _origin;
		glm::vec3 _direction;

	public:
		glm::vec3 getOrigin();
		glm::vec3 getDirection();
		void setOrigin(glm::vec3 origin);
		void setDirection(glm::vec3 direction);
		void computeReflectedRay(glm::vec3 Pi, glm::vec3 r);
		void computeRefractedRay(glm::vec3 Pi, glm::vec3 Vt, glm::vec3 N, float ior, float iorObject, float * newIOR);
};