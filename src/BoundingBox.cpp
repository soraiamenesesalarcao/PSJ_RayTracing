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

bool BoundingBox::intersect(Ray ray, glm::vec3 * tMin, glm::vec3 * tMax) {
	float tMinX, tMaxX, tMinY, tMaxY, tMinZ, tMaxZ;

	// inverting ray's direction to avoid division by zero
	glm::vec3 invertedDirection;
	invertedDirection.x = 1 / ray.getDirection().x;
	invertedDirection.y = 1 / ray.getDirection().y;
	invertedDirection.z = 1 / ray.getDirection().z;

	if (ray.getDirection().x >= 0) {
		tMinX = (_posMin.x - ray.getOrigin().x) * invertedDirection.x;   // <=>  / ray.getDirection().x;
		tMaxX = (_posMax.x - ray.getOrigin().x) * invertedDirection.x;
	}
	else {
		tMinX = (_posMax.x - ray.getOrigin().x) * invertedDirection.x;
		tMaxX = (_posMin.x - ray.getOrigin().x) * invertedDirection.x;		
	}

	if (ray.getDirection().y >= 0) {
		tMinY = (_posMin.y - ray.getOrigin().y) * invertedDirection.y;   // <=>  / ray.getDirection().y;
		tMaxY = (_posMax.y - ray.getOrigin().y) * invertedDirection.y;
	}
	else {
		tMinY = (_posMax.y - ray.getOrigin().y) * invertedDirection.y;
		tMaxY = (_posMin.y - ray.getOrigin().y) * invertedDirection.y;		
	}

	if ((tMinX > tMaxY) || (tMinY > tMaxX))
        return false;

	if (tMinY > tMinX)
        tMinX = tMinY;
    if (tMaxY < tMaxX)
        tMaxX = tMaxY;

	if (ray.getDirection().z >= 0) {
		tMinZ = (_posMin.z - ray.getOrigin().z) * invertedDirection.z;   // <=>  / ray.getDirection().z;
		tMaxZ = (_posMax.z - ray.getOrigin().z) * invertedDirection.z;
	}
	else {
		tMinZ = (_posMax.z - ray.getOrigin().z) * invertedDirection.z;
		tMaxZ = (_posMin.z - ray.getOrigin().z) * invertedDirection.z;		
	}

	if ((tMinX > tMaxZ) || (tMinZ > tMaxX))
        return false;

	if (tMinZ > tMinX)
        tMinX = tMinZ;
    if (tMaxZ < tMaxX)
        tMaxX = tMaxZ;

	/*if (tmin > r.tmin) r.tmin = tmin;
    if (tmax < r.tmax) r.tmax = tmax;*/

	tMin->x = tMinX;
	tMin->y = tMinY;
	tMin->z = tMinZ;

	tMax->x = tMaxX;
	tMax->y = tMaxY;
	tMax->z = tMaxZ;

	return true;
}