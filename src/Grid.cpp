#include "Grid.h"


Cell * Grid::getCell(int x, int y, int z) {
	int index = x + _N.x * y + _N.x * _N.y * z;
	return _cells[index];
}


glm::vec3 Grid::getN() {
	return _N;
}

glm::vec3 Grid::getW() {
	return _W;
}

BoundingBox Grid::getBoundingBox() {
	return _bb;
}


void Grid::setBoundingBox(	float pMinX, float pMinY, float pMinZ,
							float pMaxX, float pMaxY, float pMaxZ) {
	_bb.setPosMin(pMinX, pMinY, pMinZ);
	_bb.setPosMax(pMaxX, pMaxY, pMaxZ);
}


void Grid::setCells(int nObjects, int multiplyFactor) {
	_W.x = _bb.getPosMax().x - _bb.getPosMin().x;
	_W.y = _bb.getPosMax().y - _bb.getPosMin().y;
	_W.z= _bb.getPosMax().z - _bb.getPosMin().z;

	float s = glm::pow((_W.x * _W.y * _W.z) / nObjects, 1.0f / 3.0f); // raiz cubica
	_N.x = glm::ceil((multiplyFactor * _W.x) / s);
	_N.y = glm::ceil((multiplyFactor * _W.y) / s);
	_N.z = glm::ceil((multiplyFactor * _W.z) / s);

	_cells = std::vector<Cell *>(nObjects);

	int x, y, z, index;
	for(x = 0; x < _N.x; x++) {
		for(y = 0; y < _N.y; y++) {
			for(z = 0; z < _N.z; z++) {
				index = x + _N.x * y + _N.x * _N.y * z;
				_cells[index] = new Cell(index);
			}
		}
	}
}