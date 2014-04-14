#pragma once

#include "Definitions.h"
#include "Object.h"

class Cell {
	
	private:
		//int _index;
		int _x;
		int _y;
		int _z;
		std::vector<Object *> _objects;

	public:
		//Cell(int index);
		Cell(int x, int y, int z);
		int getX();
		int getY();
		int getZ();
		std::vector<Object *> getObjects();
		//int getIndex();
		//void setIndex(int index);
		bool isEmpty();
		void addObject(Object * object);
};