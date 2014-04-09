#pragma once

#include "Object.h"

class Plan : public Object{

	private:
		glm::vec3 point_1;
		glm::vec3 point_2;
		glm::vec3 point_3;

	public:
		Plan(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3, Material mtl);
		glm::vec3 getPoint1(){ return point_1; }
		glm::vec3 getPoint2(){ return point_2; }
		glm::vec3 getPoint3(){ return point_3; }

		void setBoundingBox();
		bool intersect(glm::vec3 * Pi, float * Ti, glm::vec3 * normal, Ray ray);

};