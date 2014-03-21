#pragma once

#include "Ray.h"
#include "Definitions.h"


class Camera {
    private:
		float _winWidth, _winHeight;
		Ray _ray;
		Camera();

    public:       
		~Camera();
		static Camera * getInstance();
		int GetResX();
		int GetResY();
		Ray PrimaryRay(float x, float y);
		void setResX(int w);
		void setResY(int h);
        void put();	
	};