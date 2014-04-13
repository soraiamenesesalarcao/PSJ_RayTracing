#include "Cell.h"


int Cell::getIndex() {
	return _index;
}


void Cell::setIndex(int index) {
	_index = index;
}


bool Cell::isEmpty(){
	return _objects.empty();
}


void Cell::addObject(Object * object) {
	_objects.push_back(object);
}

