#include "Scene.h"

Scene::Scene() {
}


Scene::~Scene() {
	_camera.~Camera();
	_rt.~RayTracer();
}

Scene * Scene::getInstance(){
	static Scene instance;
	return &instance;
}


Camera Scene::getCamera() {
	return _camera;
}

void Scene::init() {
	_viewpoint = new Viewpoint();
	_background = new RGB();
	// Change the nff file
	ConfigLoader::loadSceneNFF("resources/balls_low.nff", _background, &_lights, &_objects, _viewpoint);
	 _camera.init(_viewpoint);
	 _needToDraw = true;
	 _antiAliased = false;
	 _usingThreads = true;
	 _depthOfField = false;
	 _rt.setUsingDoF(_depthOfField);
}

void Scene::draw() {
	
	if(_needToDraw) {
		time_t timer1, timer2;
		int RES_X = _camera.GetResX();
		int RES_Y = _camera.GetResY(); 
		int TOTAL = RES_X * RES_Y;
		
		std::vector<RGB> image = std::vector<RGB>(TOTAL);
			
		// Ray Tracing Calculation
		time(&timer1);

		// Set the threads (if enabled)
		int nCPU = _usingThreads ? omp_get_num_procs() : 1;
		omp_set_num_threads(nCPU);
		#pragma omp parallel for schedule(static, (RES_Y) / (nCPU * nCPU))

		for (int y = 0; y < RES_Y; y++) {
			for (int x = 0; x < RES_X; x++) {
				RGB color;

				if(!_antiAliased) {				
					Ray ray =  _camera.PrimaryRay(x, y);
					_rt.incNRays();
					ray.setRayID(_rt.getNRays());
					if(_depthOfField){
						color = _rt.depthOfField(_background, _lights, _objects, &ray, 1, 1.0f, glm::normalize(_camera.computeV()), _camera);
					} else
						color = _rt.trace(_background, _lights, _objects, &ray, 1, 1.0f, glm::normalize(_camera.computeV()));
				}
				else color = _rt.monteCarlo(_camera, _background, _lights, _objects, x, y, 1);

				image[(RES_X*y) + x] = color;
			}
		}

		time(&timer2);
		float time = difftime(timer2, timer1);
		std::cout << "Time spent Ray Tracing: " << time << " seconds" << std::endl;

		// Draw Scene
		for (int y = 0; y < RES_Y; y++) {
			for (int x = 0; x < RES_X; x++) {
				glBegin(GL_POINTS);		
				glColor3f(image[(RES_X*y) + x].r, image[(RES_X*y) + x].g, image[(RES_X*y) + x].b);
				glVertex2f(x, y);		
			}
			glEnd(); 
			glFlush();
		}

		_needToDraw = false;
		printf("Terminou!\n"); 
	}
}

void Scene::update() {

	if(Input::getInstance()->keyWasReleased('G')) {		
		_needToDraw = true;
		_rt.toggleUsingGrid();
		_rt.init(_objects);
	}

	if(Input::getInstance()->keyWasReleased('S')) {		
		_needToDraw = true;
		_rt.toggleUsingSoftShadows();
	}

	if(Input::getInstance()->keyWasReleased('A')) {		
		_needToDraw = true;
		if(_antiAliased) {
			_antiAliased = false;
			std::cout << "Anti-aliasing desactivado" << std::endl;
		}
		else {
			_antiAliased = true;
			std::cout << "Anti-aliasing activado" << std::endl;
		}
	}

	if(Input::getInstance()->keyWasReleased('D')) {		
		_needToDraw = true;
		if(_depthOfField) {
			_depthOfField = false;
			std::cout << "DoF desactivado" << std::endl;
		}
		else {
			_depthOfField = true;
			std::cout << "DoF activado" << std::endl;
		}
		_rt.setUsingDoF(_depthOfField);
	}

	if(Input::getInstance()->keyWasReleased('T')) {		
		_needToDraw = true;
		if(_usingThreads) {
			_usingThreads = false;
			std::cout << "Threads desactivadas" << std::endl;
		}
		else {
			_usingThreads = true;
			std::cout << "Threads activadas" << std::endl;
		}
	}
}
