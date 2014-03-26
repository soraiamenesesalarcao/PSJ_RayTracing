#include "Scene.h"

Scene::Scene() {
}

Scene * Scene::getInstance(){
	static Scene instance;
	return &instance;
}


void Scene::init() {
	_nff = new NFF();
	ConfigLoader::loadSceneNFF("resources/balls_medium.nff", _nff);
	Camera::getInstance()->init(&(_nff->camera));
	
}

void Scene::draw() {
	int RES_X = Camera::getInstance()->GetResX();
	int RES_Y = Camera::getInstance()->GetResY(); 
	
	//para cada pixel do viewport 
	for (int y = 0; y < RES_Y; y++) {
		for (int x = 0; x < RES_X; x++) {
			//determinar em WCS o raio primario que vai do centro de projecao ao pixel
			Ray ray = Camera::getInstance()->PrimaryRay(x, y);
			RGB color = RayTracer::getInstance()->trace(_nff, ray, 1);
			glBegin(GL_POINTS);		
			glColor3f(color.r, color.g, color.b);
			glVertex2f(x, y);
			glEnd();
			glFlush(); 
		}
	}
	printf("Terminou!\n"); 
}
