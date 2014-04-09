#include "Sphere.h"

Sphere::Sphere(glm::vec3 center, float radius, Material mtl): Object(mtl){
	_center = center;
	_radius = radius;
}

void Sphere::setBoundingBox(){
	_bb.setPosMin(_center.x - _radius, _center.y - _radius, _center.z - _radius);
	_bb.setPosMax(_center.x + _radius, _center.y + _radius, _center.z + _radius);
}

/* Calculo da intersecao de um raio a uma esfera 
 * 1 - normalizar vector direccao
 * 2 - calcular o quadrado da distancia da origem do raio ao centro da esfera
 * 3 - comparar o quadrado da distancia com o quadrado do raio da esfera, se for igual retorna falso
 * 4 - calcular o B
 * ....
 */
bool Sphere::intersect(glm::vec3 * Pi, float * Ti, glm::vec3 * normal, Ray ray){
	//passo 1
	glm::vec3 D = glm::normalize(glm::vec3(ray.getDirection().x, ray.getDirection().y, ray.getDirection().z));
	glm::vec3 O = glm::vec3(ray.getOrigin().x, ray.getOrigin().y, ray.getOrigin().z);
	glm::vec3 C = _center;

	//passo 2
	float dQuad = glm::pow2(C.x - O.x) + glm::pow2(C.y - O.y) + glm::pow2(C.z - O.z);

	//passo 3
	float radiusQuad = glm::pow2(_radius);
	if(dQuad == radiusQuad){ // encontra-se sobre a superficie da esfera
		return false;
	}

	//passo 4
	float B = D.x * (C.x - O.x) + D.y * (C.y - O.y) + D.z * (C.z - O.z);
	
	//passo 5
	if(dQuad > radiusQuad){ // raio aponta para o sentido contrario a localizacao da esfera relativamente a origem do raio
		if( B < 0)
			return false;
	}

	//passo 6
	float R = glm::pow2(B) - dQuad + radiusQuad;
	if(R < 0){
		return false;
	}

	//passo 7
	if(dQuad > radiusQuad){
		*Ti = B - glm::sqrt(R);
	} 
	else if(dQuad < radiusQuad){
		*Ti = B + glm::sqrt(R);
	}

	//passo 8
	*Pi = O + D * *Ti;

	//passo 9
	float t = *Ti;
	glm::vec3 pi = *Pi;
	*normal = glm::normalize((pi - C)/_radius);

	if(dQuad < radiusQuad){ // encontra-se dentro da esfera
		*normal = *normal * -1.0f;
	}

	return true;
}