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

bool BoundingBox::intersect(Ray ray, glm::vec3 * tMin, glm::vec3 * tMax, glm::vec3 * iPoint) {
	// Kay and Kajiya algorithm
	// 1) init
	float tMinX, tMinY, tMinZ, tMaxX, tMaxY, tMaxZ, tDist = FLT_MAX, tProx = -FLT_MAX;
	
	// 2/3) the ray is parallel to the grid and is not inside it, which means it never intersects it
	if((ray.getDirection().x == 0 &&  (ray.getOrigin().x < _posMin.x || ray.getOrigin().x > _posMax.x))
		|| (ray.getDirection().y == 0 &&  (ray.getOrigin().y < _posMin.y || ray.getOrigin().y > _posMax.y))
		|| (ray.getDirection().z == 0 &&  (ray.getOrigin().z < _posMin.z || ray.getOrigin().z > _posMax.z))) {
			return false; 
	}

	// 4) calculate the intersecions

	// inverting ray's direction to avoid division by zero 
	// (will only happen if the ray is inside the grid)
	glm::vec3 invertedDirection;
	invertedDirection.x = 1 / ray.getDirection().x;
	invertedDirection.y = 1 / ray.getDirection().y;
	invertedDirection.z = 1 / ray.getDirection().z;

	tMinX = (_posMin.x - ray.getOrigin().x) * invertedDirection.x;
	tMinY = (_posMin.y - ray.getOrigin().y) * invertedDirection.y;
	tMinZ = (_posMin.z - ray.getOrigin().z) * invertedDirection.z;

	tMaxX = (_posMax.x - ray.getOrigin().x) * invertedDirection.x;
	tMaxY = (_posMax.y - ray.getOrigin().y) * invertedDirection.y;
	tMaxZ = (_posMax.z - ray.getOrigin().z) * invertedDirection.z;

	tMin->x = std::min(tMinX, tMaxX);
	tMin->y = std::min(tMinY, tMaxY);
	tMin->z = std::min(tMinZ, tMaxZ);

	tMax->x = std::max(tMinX, tMaxX);
	tMax->y = std::max(tMinY, tMaxY);
	tMax->z = std::max(tMinZ, tMaxZ);
	
	tProx = std::max(std::max(std::max(tMin->x, tMin->y), tMin->z), tProx);
	tDist = std::min(std::min(std::min(tMax->x, tMax->y), tMax->z), tDist);

	// 5/6) the ray does not intersect the box's planes or points in the opposite direction
	if(tDist > tProx || tDist < 0)  
		return false;

	// 7/8) the algorithm has found the intersections
	iPoint->x = ray.getOrigin().x + tProx * ray.getDirection().x;
	iPoint->y = ray.getOrigin().y + tProx * ray.getDirection().y;
	iPoint->z = ray.getOrigin().z + tProx * ray.getDirection().z;
	return true;
}