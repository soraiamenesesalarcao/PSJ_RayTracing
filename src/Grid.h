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
		Cell * getCell(int x, int y, int z);
		glm::vec3 getN();
		glm::vec3 getW();
		BoundingBox getBoundingBox();
		void setBoundingBox(float pMinX, float pMinY, float pMinZ,
							float pMaxX, float pMaxY, float pMaxZ);
		void setCells(int nObjects, int multiplyFactor);
};