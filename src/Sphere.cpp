#include "Sphere.h"

Sphere::Sphere(glm::vec3 center, float radius, Material mtl): Object(mtl){
	_center = center;
	_radius = radius;
}

void Sphere::setBoundingBox(){
	_bb = new BoundingBox();
	_bb->setPosMin(_center.x - _radius, _center.y - _radius, _center.z - _radius);
	_bb->setPosMax(_center.x + _radius, _center.y + _radius, _center.z + _radius);
}

/* Ray-sphere intersection 
 * 1 - Normalize ray direction
 * 2 - Compute the square of the distance between the ray origin and the sphere center
 * 3 - Compare the square of the distance with the square of the sphere radius 
 *		- if they're equal, return false
 * 4 - Compute B
 * ....
 */
bool Sphere::intersect(glm::vec3 * Pi, float * Ti, glm::vec3 * normal, Ray ray){
	//step 1
	glm::vec3 D = glm::normalize(glm::vec3(ray.getDirection().x, ray.getDirection().y, ray.getDirection().z));
	glm::vec3 O = glm::vec3(ray.getOrigin().x, ray.getOrigin().y, ray.getOrigin().z);
	glm::vec3 C = _center;

	//step 2
	float dQuad = glm::pow2(C.x - O.x) + glm::pow2(C.y - O.y) + glm::pow2(C.z - O.z);

	//step 3
	float radiusQuad = glm::pow2(_radius);
	if(dQuad == radiusQuad){ // it is over sphere surface
		return false;
	}

	//step 4
	float B = D.x * (C.x - O.x) + D.y * (C.y - O.y) + D.z * (C.z - O.z);
	
	//step 5
	if(dQuad > radiusQuad){ // ray's directtion is opposite to sphere location  
		if( B < 0)			
			return false;
	}

	//step 6
	float R = glm::pow2(B) - dQuad + radiusQuad;
	if(R < 0){
		return false;
	}

	//step 7
	if(dQuad > radiusQuad){
		*Ti = B - glm::sqrt(R);
	} 
	else if(dQuad < radiusQuad){
		*Ti = B + glm::sqrt(R);
	}

	//step 8 - Compute intersection point
	*Pi = O + D * *Ti;

	//step 9
	float t = *Ti;
	glm::vec3 pi = *Pi;
	*normal = glm::normalize((pi - C)/_radius);

	if(dQuad < radiusQuad){ // it is inside the sphere
		*normal = *normal * -1.0f;
	}

	return true;
}