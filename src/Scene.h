#pragma once

#include "Camera.h"
#include "Definitions.h"
#include "ConfigLoader.h"
#include "RayTracer.h"

class Scene {

	private:
		Camera * _camera;
		NFF * _nff;
		Scene();

	public:
		static Scene * getInstance();
		void init();
		void draw();


};