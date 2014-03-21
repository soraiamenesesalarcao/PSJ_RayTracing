#pragma once

#include "Definitions.h"

class Ray {

	private:
		float _x;
		float _y;

	public:
		void setX(int x);
		void setY(int y);
		float getX();
		float getY();
};
