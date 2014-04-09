#include "Object.h"

Object::Object(Material mtl){
	_mtl = mtl;
}

Material Object::getMaterial(){
	return _mtl;
}

BoundingBox Object::getBoundingBox() {
	return _bb;
}