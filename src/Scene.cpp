#include "Scene.h"

Scene::Scene() {
}

Scene * Scene::getInstance(){
	static Scene instance;
	return &instance;
}


void Scene::init() {
	_camera = new Viewpoint();
	_background = new RGB();
	ConfigLoader::loadSceneNFF("resources/balls_high.nff", _background, &_lights, &_objects, _camera);
	
	Camera::getInstance()->init(_camera);

}

void Scene::draw() {
	time_t timer1, timer2;
	int RES_X = Camera::getInstance()->GetResX();
	int RES_Y = Camera::getInstance()->GetResY(); 
	int TOTAL = RES_X * RES_Y;
	int nCPU = omp_get_num_procs();
	std::vector<RGB> image = std::vector<RGB>(TOTAL);

	// Ray Tracing Calculation
	time(&timer1);
	omp_set_num_threads(nCPU);
	#pragma omp parallel for schedule(static, (RES_Y) / (nCPU * nCPU))	
	for (int y = 0; y < RES_Y; y++) {
		for (int x = 0; x < RES_X; x++) {
			//determinar em WCS o raio primario que vai do centro de projecao ao pixel
			Ray ray = Camera::getInstance()->PrimaryRay(x, y);
			RGB color = RayTracer::getInstance()->trace(_background, _lights, _objects, ray, 1, 1.0f);
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
			glEnd();			
		}
		glFlush();
	}

	printf("Terminou!\n"); 
}
