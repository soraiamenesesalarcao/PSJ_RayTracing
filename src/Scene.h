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
		Camera _camera2;
		RayTracer _rt;
		Scene();

	public:
		~Scene();
		static Scene * getInstance();
		Camera getCamera();
		void init();
		void draw();
		RGB monteCarlo(float x, float y, int depth);
};