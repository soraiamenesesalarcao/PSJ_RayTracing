#pragma once

#include "Definitions.h"
#include "Object.h"

class Cell {
	
	private:
		int _index;
		std::vector<Object *> _objects;

	public:
		Cell(int index);
		int getIndex();
		void setIndex(int index);
		bool isEmpty();
		void addObject(Object * object);
};