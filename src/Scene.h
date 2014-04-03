#pragma once

#include "Camera.h"
#include "Definitions.h"
#include "ConfigLoader.h"
#include "RayTracer.h"
#include "Object.h"
#include "Light.h"
#include "Plan.h"
#include "Ray.h"

class Scene {

	private:
		RGB * _background;
		Viewpoint * _camera;
		std::vector<Light> _lights;
		std::vector<Object*> _objects;
		Scene();

	public:
		static Scene * getInstance();
		void init();
		void draw();
};