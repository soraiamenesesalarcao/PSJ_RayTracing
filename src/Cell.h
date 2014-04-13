#pragma once

#include "Definitions.h"
#include "Object.h"
#include "Sphere.h"
#include "Polygon.h"

class Cell {
	
	private:
		int _index;
		std::vector<Object *> _objects;

	public:
		int getIndex();
		void setIndex(int index);
		bool isEmpty();
		void addObject(Object * object);
};