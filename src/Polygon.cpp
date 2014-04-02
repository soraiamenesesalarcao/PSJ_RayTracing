#include "Polygon.h"

Polygon::Polygon(std::vector<Vertex> vertices, Material mtl) : Object(mtl){
	_vertices = vertices;

}

bool Polygon::intersect(glm::vec3 * Pi, float * Ti, glm::vec3 * normal, Ray ray){
	glm::vec3 v1 = glm::vec3(_vertices[0].vx, _vertices[0].vy, _vertices[0].vz);
	glm::vec3 v2 = glm::vec3(_vertices[1].vx, _vertices[1].vy, _vertices[1].vz);
	glm::vec3 v3 = glm::vec3(_vertices[2].vx, _vertices[2].vy, _vertices[2].vz);

	return intersectPolygonAux(Pi, Ti, normal, ray, v1, v2, v3);
}

bool Polygon::intersectPolygonAux(glm::vec3 * Pi, float * Ti, glm::vec3 * normal, Ray ray, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3){

	glm::vec3 N = glm::cross((v2 - v1), (v3 - v1));

	// calcular o ponto de intersecao com o plano do poligono
	float NdotD = glm::dot(N, ray.direction);
	if(NdotD == 0){
		return false;
	}

	glm::vec3 sub = ray.origin - v1;
	float NdotO = glm::dot(N, sub);
	float t = - (NdotO/ NdotD);
	if(t < 0){
		return false;
	}	
	*Pi = ray.origin + ray.direction*t;

	// ver se o ponto de intersecao com o plano esta dentro do poligono
	if (polygonContainsPoint(v1, v2,v3, N, *Pi)) {
		*Ti = t;
		*normal = glm::normalize(N);
		return true;
	}
	return false; 
}

bool Polygon::polygonContainsPoint(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, 
										glm::vec3 normal, glm::vec3 point) {
	glm::vec2 projectedV1, projectedV2, projectedV3, projectedPoint;
	glm::vec3 absNormal = glm::vec3(std::abs(normal.x), std::abs(normal.y), std::abs(normal.z));
	float axisToIgnore = std::max(std::max(absNormal.x, absNormal.y), absNormal.z);
	// 2D Projection
	if(axisToIgnore == absNormal.x) {
		projectedV1 = glm::vec2(v1.y, v1.z);
		projectedV2 = glm::vec2(v2.y, v2.z);
		projectedV3 = glm::vec2(v3.y, v3.z);
		projectedPoint = glm::vec2(point.y, point.z);
	}
	else if(axisToIgnore == absNormal.y) {
		projectedV1 = glm::vec2(v1.x, v1.z);
		projectedV2 = glm::vec2(v2.x, v2.z);
		projectedV3 = glm::vec2(v3.x, v3.z);
		projectedPoint = glm::vec2(point.x, point.z);
	}
	else {
		projectedV1 = glm::vec2(v1.x, v1.y);
		projectedV2 = glm::vec2(v2.x, v2.y);
		projectedV3 = glm::vec2(v3.x, v3.y);
		projectedPoint = glm::vec2(point.x, point.y);
	}

	// Winding Number Test
	std::vector<glm::vec2> vertices;
	vertices.push_back(projectedV1);
	vertices.push_back(projectedV2);
	vertices.push_back(projectedV3);
	vertices.push_back(projectedV1);
	int wn = windingNumber(vertices, projectedPoint);		
	if (wn != 0)
		return true;
	else
		return false;
}



float Polygon::isLeft(glm::vec2 P0, glm::vec2 P1, glm::vec2 P2)
{
    return ((P1.x - P0.x) * (P2.y - P0.y) - (P2.x -  P0.x) * (P1.y - P0.y));
}

int Polygon::windingNumber(std::vector<glm::vec2> vertices, glm::vec2 point) {
	int wn = 0;
	std::vector<glm::vec2>::iterator v;
	std::vector<glm::vec2>::iterator v1;
	
	for(v = vertices.begin(), v1 = vertices.begin() + 1; v != vertices.end() - 1; v++, v1++) {
		if (v->y <= point.y) {
			if (v1->y  > point.y) {  
				if (isLeft( *v, *v1, point) > 0) {
					++wn;     
				}
			}
		}
		else {                        
			if (v1->y  <= point.y) {
				if (isLeft( *v, *v1, point) < 0) { 
					--wn;                  
				}
			}
		}
	}
	return wn;
}

