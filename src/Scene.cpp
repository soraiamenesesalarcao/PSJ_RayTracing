#include "Scene.h"

Scene::Scene() {
}

Scene * Scene::getInstance(){
	static Scene instance;
	return &instance;
}

Color Scene::rayTracing(Ray ray, float depth, float IoRefraction) {
	Color c;
	c.setR(1.0);
	c.setG(0.0);
	c.setB(0.0);
	return c;
}

void Scene::init() {
	_nff = new NFF();
	ConfigLoader::loadSceneNFF("resources/jap.nff", *_nff);	
}

void Scene::draw() {
	int RES_X = Camera::getInstance()->GetResX();
	int RES_Y = Camera::getInstance()->GetResY(); 

	for (int y = 0; y < RES_Y; y++) {
		for (int x = 0; x < RES_X; x++) {
			Ray ray = Camera::getInstance()->PrimaryRay(x, y);
			Color color= rayTracing(ray, 1, 1.0 ); //depth=1, ior=1.0
			glBegin(GL_POINTS);
			glColor3f(color.r(), color.g(), color.b());
			glVertex2f(x, y);
			glEnd();
			glFlush(); 
		}
	}
	printf("Terminou!\n"); 
}
