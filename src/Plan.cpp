#include "Plan.h"

Plan::Plan(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3, Material mtl) : Object(mtl){
	point_1 = point1;
	point_2 = point2;
	point_3 = point3;
}

void Plan::setBoundingBox(){
	_bb = NULL;
}

/* Ray-plan intersection
 * (point A, B, C)
 * 1 - Compute plan's normal --> N = AB x AC = (B - A) x (C - A)
 * 2 - Compute dot product between normal and ray's direction
 *     If equals 0 => ray is parallel to the plan => no intersection
 * 3 - Compute t = - (N.origin / N.direction) 
 */
bool Plan::intersect(glm::vec3 * Pi, float * Ti, glm::vec3 * normal, Ray ray){
	glm::vec3 A = point_1; //glm::vec3(point_1.x, plan.point_1.y, plan.point_1.z);
	glm::vec3 B = point_2; //glm::vec3(point_2.x, plan.point_2.y, plan.point_2.z);
	glm::vec3 C = point_3; //glm::vec3(point_3.x, plan.point_3.y, plan.point_3.z);
	glm::vec3 N = glm::cross((B - A), (C - A));

	float NdotD = glm::dot(N, ray.getDirection());
	if(NdotD == 0){
		return false;
	}

	glm::vec3 sub = ray.getOrigin() - A;
	float NdotO = glm::dot(N, sub);
	float t = - (NdotO/ NdotD);
	if(t < 0){
		return false;
	}

	// Compute intersection point
	*Pi = ray.getOrigin() + ray.getDirection()*t;
	*Ti = t;
	*normal = glm::normalize(N);
	return true;
}