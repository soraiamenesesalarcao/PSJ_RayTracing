#pragma once

#include "Definitions.h"
#include "Object.h"
#include "Cell.h"

class Grid {
	
	private:
		std::vector<Cell *> _cells;
		int _Nx;
		int _Ny;
		int _Nz;

	public:
		Grid(float wx, float wy, float wz, int nObjects, int multiplyFactor);
		Cell * getCell(int x, int y, int z);
		void init();
};