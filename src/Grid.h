#pragma once

#include "Definitions.h"
#include "Cell.h"

class Grid {
	
	private:
		std::vector<Cell *> _cells;
		BoundingBox _bb;
		glm::vec3 _N;
		glm::vec3 _W;

	public:		
		glm::vec3 getN();
		glm::vec3 getW();

		Cell * getCell(int x, int y, int z);
		Cell * getStartingCell(Ray ray, glm::vec3 rayT);
		Cell * cellTraversal(	Cell * startingCell, float * tMax,
								glm::vec3 * rayTmin, glm::vec3 * rayTmax,
								int stepX, int stepY, int stepZ);

		BoundingBox getBoundingBox();
		void setBoundingBox(float pMinX, float pMinY, float pMinZ,
							float pMaxX, float pMaxY, float pMaxZ);
		void setCells(int nObjects, int multiplyFactor);
		
		void computeBoundingBoxes(std::vector<Object*> objects);
		void addObjectsToGrid(std::vector<Object*> objects);
		
};