#pragma once

#include "Color.h"
#include "Camera.h"
#include "Definitions.h"
#include "ConfigLoader.h"

class Scene {

	private:
		Camera * _camera;
		NFF * _nff;
		Scene();

	public:
		static Scene * getInstance();
		Color rayTracing(Ray ray, float depth, float IoRefraction);
		void init();
		void draw();


};