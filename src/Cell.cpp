#include "Cell.h"

//Cell::Cell(int index) {
//	_index = index;
//}
//
//int Cell::getIndex() {
//	return _index;
//}
//
//
//void Cell::setIndex(int index) {
//	_index = index;
//}


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

