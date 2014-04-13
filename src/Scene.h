#pragma once

#include "Camera.h"
#include "Definitions.h"
#include "ConfigLoader.h"
#include "RayTracer.h"
#include "Object.h"
#include "Light.h"
#include "Plan.h"
#include "Ray.h"
#include "Input.h"
#include "Grid.h"

class Scene {

	private:
		RGB * _background;
		Viewpoint * _viewpoint;
		std::vector<Light> _lights;
		std::vector<Object*> _objects;
		Camera _camera;
		RayTracer _rt;
		Grid _grid;
		bool _needToDraw;
		bool _usingGrid;
		Scene();

	public:
		~Scene();
		static Scene * getInstance();
		Camera getCamera();
		void init();
		void draw();
		void update();
		void computeBoundingBoxes();
		void addObjectsToGrid();
		RGB monteCarlo(float x, float y, int depth);
		
};