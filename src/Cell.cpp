#include "Cell.h"

Cell::Cell(int x, int y, int z) {
	_x = x;
	_y = y;
	_z = z;
}


int Cell::getX() {
	return _x;
}


int Cell::getY() {
	return _y;
}


int Cell::getZ() {
	return _z;
}


std::vector<Object *> Cell::getObjects() {
	return _objects;
}


bool Cell::isEmpty(){
	return _objects.empty();
}


void Cell::addObject(Object * object) {
	_objects.push_back(object);
}

