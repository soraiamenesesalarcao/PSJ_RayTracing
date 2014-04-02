#pragma once

#include "Definitions.h"

class Light{
	
	private:
		glm::vec3 _position;
		RGB _color;

	public:
		Light(glm::vec3 position, RGB color);
		glm::vec3 getPosition();
		RGB getColor();

		void setPosition(float x, float y, float z);


};