#pragma once

#include "Ray.h"
#include "Definitions.h"


class Camera {
    private:
		float _winWidth, _winHeight; //ResX, ResY
		Position eye; //from
		Position at;
		Position up;
		float fovy; //angle
		float near; //hither
		float w, h;

		Ray _ray;

		//uvn frame
		Position xe; 
		Position ye;
		Position ze;

		float distance;
		Camera();

    public: 
		
		~Camera();
		static Camera * getInstance();
		void init(Viewpoint * viewpoint);
		int GetResX();
		int GetResY();
		Ray PrimaryRay(float x, float y);
		void setResX(int w);
		void setResY(int h);
        void put();	
	};