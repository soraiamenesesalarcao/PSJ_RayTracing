#include "Grid.h"


Grid::Grid(float wx, float wy, float wz, int nObjects, int multiplyFactor) {
	_cells = std::vector<Cell *>(nObjects);
	int s = glm::pow((wx * wy * wz) / nObjects, 1.0f / 3.0f); // raiz cubica
	_Nx = glm::ceil((multiplyFactor * wx) / s);
	_Ny = glm::ceil((multiplyFactor * wy) / s);
	_Nz = glm::ceil((multiplyFactor * wz) / s);
}


Cell * Grid::getCell(int x, int y, int z) {
	int index = x + _Nx * y + _Nx * _Ny * z;
	return _cells[index];
}


void Grid::init() {
	int x, y, z, index;
	for(x = 0; x < _Nx; x++) {
		for(y = 0; y < _Ny; y++) {
			for(z = 0; z < _Nz; z++) {
				index = x + _Nx * y + _Nx * _Ny * z;
				_cells[index] = new Cell(index);
			}
		}
	}
}