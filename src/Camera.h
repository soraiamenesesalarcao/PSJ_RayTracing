#pragma once

#include "Ray.h"


class Camera {
    private:
		float _winWidth, _winHeight; //ResX, ResY
		glm::vec3 eye; //from
		glm::vec3 at;
		glm::vec3 up;
		float fovy; //angle
		float near; //hither
		float w, h;

		//uvn frame
		glm::vec3 xe; 
		glm::vec3 ye;
		glm::vec3 ze;

		float df;

    public: 

		Camera();
		~Camera();
		void init(Viewpoint * viewpoint);
		int GetResX();
		int GetResY();
		glm::vec3 computeV();
		Ray PrimaryRay(float x, float y);
		void setResX(int w);
		void setResY(int h);
        void put();	
		
		glm::vec3 getEye(){ return eye; }
		glm::vec3 getXe(){ return xe; }
		glm::vec3 getYe(){ return ye; }
	};