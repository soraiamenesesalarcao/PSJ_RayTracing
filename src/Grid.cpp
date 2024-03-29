#include "Grid.h"


glm::vec3 Grid::getN() {
	return _N;
}


glm::vec3 Grid::getW() {
	return _W;
}


Cell * Grid::getCell(int x, int y, int z) {
	int index = x + _N.x * y + _N.x * _N.y * z;
	return _cells[index];
}

// Get the cell where the ray intersects the grid's bounding box
Cell * Grid::getStartingCell(Ray ray, glm::vec3 iPoint) {
	int cx, cy, cz;
	glm::vec3 gbbMin = getBoundingBox().getPosMin();


	if(	ray.getOrigin().x < _bb.getPosMin().x || ray.getOrigin().x > _bb.getPosMax().x
		|| ray.getOrigin().y < _bb.getPosMin().y || ray.getOrigin().y > _bb.getPosMax().y
		|| ray.getOrigin().z < _bb.getPosMin().z || ray.getOrigin().z > _bb.getPosMax().z) {

			// // the ray's origin is outside the grid's bb: compute adjacent voxel's coords
			cx = glm::clamp(((iPoint.x - gbbMin.x) * _N.x) / _W.x, 0.0f, _N.x - 1);
			cy = glm::clamp(((iPoint.y - gbbMin.y) * _N.y) / _W.y, 0.0f, _N.y - 1);
			cz = glm::clamp(((iPoint.z - gbbMin.z) * _N.z) / _W.z, 0.0f, _N.z - 1);
	}
	else { // the ray's origin is inside the grid's bb
		cx = glm::clamp(((ray.getOrigin().x - gbbMin.x) * _N.x) / _W.x, 0.0f, _N.x - 1);
		cy = glm::clamp(((ray.getOrigin().y - gbbMin.y) * _N.y) / _W.y, 0.0f, _N.y - 1);
		cz = glm::clamp(((ray.getOrigin().z - gbbMin.z) * _N.z) / _W.z, 0.0f, _N.z - 1);
	}	
	return getCell(cx, cy, cz);
}

BoundingBox Grid::getBoundingBox() {
	return _bb;
}


void Grid::setBoundingBox(	float pMinX, float pMinY, float pMinZ,
							float pMaxX, float pMaxY, float pMaxZ) {
	_bb.setPosMin(pMinX, pMinY, pMinZ);
	_bb.setPosMax(pMaxX, pMaxY, pMaxZ);
}

// Initialize grid's cells
void Grid::setCells(int nObjects, int multiplyFactor) {
	_W.x = _bb.getPosMax().x - _bb.getPosMin().x;
	_W.y = _bb.getPosMax().y - _bb.getPosMin().y;
	_W.z= _bb.getPosMax().z - _bb.getPosMin().z;

	float s = glm::pow((_W.x * _W.y * _W.z) / nObjects, 1.0f / 3.0f); // raiz cubica

	_N.x = glm::ceil(multiplyFactor * (_W.x / s));
	_N.y = glm::ceil(multiplyFactor * (_W.y / s));
	_N.z = glm::ceil(multiplyFactor * (_W.z / s));

	_cells = std::vector<Cell *>(_N.x * _N.y * _N.z);

	int x, y, z, index;
	for(x = 0; x < _N.x; x++) {
		for(y = 0; y < _N.y; y++) {
			for(z = 0; z < _N.z; z++) {
				index = x + _N.x * y + _N.x * _N.y * z;
				_cells[index] = new Cell(x, y, z);
			}
		}
	}
}

// Compute both the grid's bounding box and the objects's bounding box
void Grid::computeBoundingBoxes(std::vector<Object*> objects) {
	glm::vec3 pMin, pMax;

	// init
	objects[0]->setBoundingBox();
	if(objects[0]->getBoundingBox() != NULL) {
		pMin = objects[0]->getBoundingBox()->getPosMin();
		pMax = objects[0]->getBoundingBox()->getPosMax();
	}

	for(std::size_t i = 1; i < objects.size(); i++){
		objects[i]->setBoundingBox();
		if(objects[i]->getBoundingBox() != NULL) {
			pMin.x = std::min(objects[i]->getBoundingBox()->getPosMin().x, pMin.x);
			pMin.y = std::min(objects[i]->getBoundingBox()->getPosMin().y, pMin.y);
			pMin.z = std::min(objects[i]->getBoundingBox()->getPosMin().z, pMin.z);

			pMax.x = std::max(objects[i]->getBoundingBox()->getPosMax().x, pMax.x);
			pMax.y = std::max(objects[i]->getBoundingBox()->getPosMax().y, pMax.y);
			pMax.z = std::max(objects[i]->getBoundingBox()->getPosMax().z, pMax.z);
		}
	}

	pMin.x -= EPSILON;
	pMin.y -= EPSILON;
	pMin.z -= EPSILON;

	pMax.x += EPSILON;
	pMax.y += EPSILON;
	pMax.z += EPSILON;

	setBoundingBox(pMin.x, pMin.y, pMin.z, pMax.x, pMax.y, pMax.z);
}

// add the objects to each grid's cell
void Grid::addObjectsToGrid(std::vector<Object*> objects) {
	glm::vec3 obbMin, obbMax, gbbMin;
	int iMinX, iMinY, iMinZ, iMaxX, iMaxY, iMaxZ, x, y, z;
	
	gbbMin = getBoundingBox().getPosMin();

	setCells(objects.size(), MULTIPLY_FACTOR);	

	for(std::size_t i = 0; i < objects.size(); i++){
		if(objects[i]->getBoundingBox() != NULL) {
			obbMin = objects[i]->getBoundingBox()->getPosMin();
			obbMax = objects[i]->getBoundingBox()->getPosMax();

			iMinX = glm::clamp((obbMin.x - gbbMin.x) * _N.x / _W.x, 0.0f, _N.x - 1);
			iMinY = glm::clamp((obbMin.y - gbbMin.y) * _N.y / _W.y, 0.0f, _N.y - 1);
			iMinZ = glm::clamp((obbMin.z - gbbMin.z) * _N.z / _W.z, 0.0f, _N.z - 1);

			iMaxX = glm::clamp((obbMax.x - gbbMin.x) * _N.x / _W.x, 0.0f, _N.x - 1);
			iMaxY = glm::clamp((obbMax.y - gbbMin.y) * _N.y / _W.y, 0.0f, _N.y - 1);
			iMaxZ = glm::clamp((obbMax.z - gbbMin.z) * _N.z / _W.z, 0.0f, _N.z - 1);
		}
		else {
			iMinX = 0;
			iMinY = 0;
			iMinZ = 0;
			iMaxX = 0;
			iMaxY = 0;
			iMaxZ = 0;
		}		
		for(z = iMinZ; z <= iMaxZ; z++) {
			for(y = iMinY; y <= iMaxY; y++) {
				for(x = iMinX; x <= iMaxX; x++) {
					getCell(x, y, z)->addObject(objects[i]);					
				}
			}
		}
	}	
}