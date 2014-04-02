#pragma once

#include "Definitions.h"

class Material{

	private:
		RGB _color;
		float _kd;
		float _ks;
		float _shine;
		float _t;
		float _indexRefraction;

	public:
		Material();
		Material(RGB color, float kd, float ks, float shine, float t, float indexRefraction);

		float getKd();
		float getKs();
		float getShine();
		float getT();
		float getIndexRefraction();
		RGB getColor();

		void setKd(float kd);
		void setKs(float ks);
		void setShine(float shine);
		void setT(float t);
		void setIndexRefraction(float indexRefraction);
		void setColor(RGB color);

};

