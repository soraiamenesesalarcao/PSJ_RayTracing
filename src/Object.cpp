#include "Object.h"


Object::Object(Material mtl){
	_mtl = mtl;
	_lastRayID = 0;
}


Material Object::getMaterial(){
	return _mtl;
}


BoundingBox Object::getBoundingBox() {
	return _bb;
}


int Object::getLastRayID() {
	return _lastRayID;
}


void Object::setLastRayID(int ID) {
	_lastRayID = ID;
}

float Object::getLastTi() {
	return _lastTi;
}

void Object::setLastTi(float Ti) {
	_lastTi = Ti;
}