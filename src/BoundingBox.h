#pragma once

#include "Definitions.h"

class BoundingBox {

	private:
		glm::vec3 _position;
		
	public:
		BoundingBox();
		glm::vec3 getPosition();
		void setPosition(float x, float y, float z);

		
};