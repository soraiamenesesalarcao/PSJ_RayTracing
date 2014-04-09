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
	ConfigLoader::loadSceneNFF("resources/balls_medium.nff", _background, &_lights, &_objects, _camera);
	
	Camera::getInstance()->init(_camera);

}

void Scene::draw() {
	time_t timer1, timer2;
	int nCPU = omp_get_num_procs();
	omp_set_num_threads(nCPU);
	std::cout << "CPU: " << nCPU << std::endl;
	std::cout << "nthreads: " << omp_get_num_threads() << std::endl;
	int RES_X = Camera::getInstance()->GetResX();
	int RES_Y = Camera::getInstance()->GetResY(); 
	
	time(&timer1);
	//para cada pixel do viewport 
	#pragma omp parallel for schedule(static, (RES_Y) / (nCPU * nCPU))	
	for (int y = 0; y < RES_Y; y++) {
		for (int x = 0; x < RES_X; x++) {
			//determinar em WCS o raio primario que vai do centro de projecao ao pixel
			Ray ray = Camera::getInstance()->PrimaryRay(x, y);
			RGB color = RayTracer::getInstance()->trace(_background, _lights, _objects, ray, 1, 1.0f);
			glBegin(GL_POINTS);		
			glColor3f(color.r, color.g, color.b);
			glVertex2f(x, y);
			glEnd();			
		}
		glFlush();
	}	
	time(&timer2);
	float time = difftime(timer2, timer1);
	std::cout << "Time spent drawing: " << time << " seconds" << std::endl;
	printf("Terminou!\n"); 
}
