#include "BoundingBox.h"

BoundingBox::BoundingBox() {
	
}

glm::vec3 BoundingBox::getPosition() {
	return _position;
}

void BoundingBox::setPosition(float x, float y, float z) {
	_position = glm::vec3(x, y, z);
}