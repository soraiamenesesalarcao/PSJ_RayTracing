#pragma once

#include "Definitions.h"

class Color {

	private:
		float _r;
		float _g;
		float _b;

	public:
		float r();
		float g();
		float b();
		void setR(float r);
		void setG(float g);
		void setB(float b);
};