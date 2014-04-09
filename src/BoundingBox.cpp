#include "BoundingBox.h"

BoundingBox::BoundingBox() {
	
}

glm::vec3 BoundingBox::getPosMin() {
	return _posMin;
}

glm::vec3 BoundingBox::getPosMax() {
	return _posMax;
}

void BoundingBox::setPosMin(float x, float y, float z) {
	_posMin = glm::vec3(x, y, z);
}

void BoundingBox::setPosMax(float x, float y, float z) {
	_posMax = glm::vec3(x, y, z);
}