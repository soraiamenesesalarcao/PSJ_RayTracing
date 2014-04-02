#pragma once

#include "Object.h"

class Polygon : public Object{

	private:
		std::vector<Vertex> _vertices;

	public:
		Polygon(std::vector<Vertex> vertices, Material mtl);
		std::vector<Vertex> getVertices() { return _vertices; }

		bool intersect(glm::vec3 * Pi, float * Ti, glm::vec3 * normal, Ray ray);

		float isLeft(glm::vec2 P0, glm::vec2 P1, glm::vec2 P2);
		int windingNumber(std::vector<glm::vec2> vertices, glm::vec2 point);
		bool polygonContainsPoint(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 normal, glm::vec3 point);
		bool intersectPolygonAux(glm::vec3 * Pi, float * Ti, glm::vec3 * normal, Ray ray, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);

};