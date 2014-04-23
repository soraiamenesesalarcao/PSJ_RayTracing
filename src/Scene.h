#pragma once

#include "Camera.h"
#include "Definitions.h"
#include "ConfigLoader.h"
#include "RayTracer.h"
#include "Object.h"
#include "Light.h"
#include "Ray.h"
#include "Input.h"

class Scene {

	private:
		RGB * _background;
		Viewpoint * _viewpoint;
		std::vector<Light> _lights;
		std::vector<Object*> _objects;
		Camera _camera;
		RayTracer _rt;
		bool _needToDraw;
		bool _antiAliased;
		bool _usingThreads;
		bool _depthOfField;
		Scene();

	public:
		~Scene();
		static Scene * getInstance();
		Camera getCamera();
		void init();
		void draw();
		void update();		
};