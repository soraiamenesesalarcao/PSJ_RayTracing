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
	ConfigLoader::loadSceneNFF("resources/jap.nff", _nff);
	Camera::getInstance()->init(&(_nff->camera));
	//std::cout << "Background: [ " << _nff->background.r << " " << _nff->background.g << " " << _nff->camera.from.pz << std::endl;
	//std::cout << "Light: x: " << _nff->lights[0].position.px << " y: " <<  _nff->lights[0].position.py  << " z: " <<  _nff->lights[0].position.pz
	//					<< " r: " <<  _nff->lights[0].color.r << " g: " <<  _nff->lights[0].color.g << " b: " <<  _nff->lights[0].color.b << std::endl;
}

void Scene::draw() {
	int RES_X = Camera::getInstance()->GetResX();
	int RES_Y = Camera::getInstance()->GetResY(); 

	//para cada pixel do viewport 
	for (int y = 0; y < RES_Y; y++) {
		for (int x = 0; x < RES_X; x++) {
			//determinar em WCS o raio primario que vai do centro de projecao ao pixel
			Ray ray = Camera::getInstance()->PrimaryRay(x, y);
			Color color= rayTracing(ray, 1, 1.0); //depth=1, ior=1.0
			glBegin(GL_POINTS);
			glColor3f(color.r(), color.g(), color.b());
			glVertex2f(x, y);
			glEnd();
			glFlush(); 
		}
	}
	printf("Terminou!\n"); 
}
