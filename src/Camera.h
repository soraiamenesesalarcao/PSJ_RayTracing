#pragma once

#include "Definitions.h"


class Camera {
    private:
		float _winWidth, _winHeight; //ResX, ResY
		glm::vec3 eye; //from
		glm::vec3 at;
		glm::vec3 up;
		float fovy; //angle
		float near; //hither
		float w, h;

		Ray _ray;

		//uvn frame
		glm::vec3 xe; 
		glm::vec3 ye;
		glm::vec3 ze;

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