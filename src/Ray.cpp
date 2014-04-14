#include "Ray.h"


int Ray::getRayID() {
	return _rayID;
}


void Ray::setRayID(int ID) {
	_rayID = ID;
}


glm::vec3 Ray::getOrigin() {
	return _origin;
}


glm::vec3 Ray::getDirection() {
	return _direction;
}


void Ray::setOrigin(glm::vec3 origin) {
	_origin = origin;
}


void Ray::setDirection(glm::vec3 direction) {
	_direction = direction;
}


void Ray::computeReflectedRay(glm::vec3 Pi, glm::vec3 r) {
	_origin = Pi + (EPSILON * r);
	_direction = r;
}


void Ray::computeRefractedRay(glm::vec3 Pi, glm::vec3 Vt, glm::vec3 N, float ior, float iorObject, float * newIOR) {
	float sinThetaI, sinThetaT, cosThetaT;

	if(ior != 1.0f) 
		*newIOR = 1.0f;
	else *newIOR = iorObject;

	glm::vec3 t = glm::normalize(Vt); 
	glm::vec3 rt;

	sinThetaI = glm::sqrt(glm::pow2(Vt.x) + glm::pow2(Vt.y) + glm::pow2(Vt.z));
	sinThetaT = (ior/(*newIOR)) * sinThetaI;

	float sinQuad = sinThetaT*sinThetaT;

	cosThetaT = 0.0f;
	if(sinQuad <= 1){
		cosThetaT = glm::sqrt(1.0f - sinQuad);

		rt = sinThetaT * t + cosThetaT*(-N);

		_origin = Pi + (EPSILON * rt);
		_direction = glm::normalize(rt);
	
	}	
	else _origin.x = NULL;
}