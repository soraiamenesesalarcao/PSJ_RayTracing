#pragma once

#include "Definitions.h" 
#include "Material.h" 

class Ray {

	private:
		int _rayID;
		glm::vec3 _origin;
		glm::vec3 _direction;
		Material _winnerMaterial;
		glm::vec3 _winnerPi;
		glm::vec3 _winnerNormal;
		float _winnerTi;


	public:
		int getRayID();
		void setRayID(int ID);
		bool equalTo(int ID);
		void setWinnerData(Material winnerMaterial, glm::vec3 winnerPi, glm::vec3 winnerNormal, float winnerTi);
		Material getWinnerMaterial();
		glm::vec3 getWinnerPi();
		glm::vec3 getWinnerNormal();
		float getWinnerTi();
		glm::vec3 getOrigin();
		glm::vec3 getDirection();
		void setOrigin(glm::vec3 origin);
		void setDirection(glm::vec3 direction);
		void computeReflectedRay(glm::vec3 Pi, glm::vec3 r);
		void computeRefractedRay(glm::vec3 Pi, glm::vec3 Vt, glm::vec3 N, float ior, float iorObject, float * newIOR);
};