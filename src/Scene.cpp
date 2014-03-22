#include "Scene.h"

Scene::Scene() {
}

Scene * Scene::getInstance(){
	static Scene instance;
	return &instance;
}


// ciclo principal
Color Scene::rayTracing(Ray ray, float depth, float IoRefraction) {
	Color c;
	RayTracer rt;
	//determinar a intersecao mais proxima com um objecto
	if(!rt.intersect(_nff->planes[0], ray)){ // se nao interceptar devolve a cor do background
		c.setR(_nff->background.r);
		c.setG(_nff->background.g);
		c.setB(_nff->background.b);
	}
	else{
		//color = termo ambiente do material do objecto
		c.setR(_nff->planes[0].mtl.color.r);
		c.setG(_nff->planes[0].mtl.color.g);
		c.setB(_nff->planes[0].mtl.color.b);
		//calcular a normal no ponto de interseccao
		//no caso do plan a normal é a equação do plano
		glm::vec3 normal = rt.getNormal();
	}

	//falta interseccao para o resto dos objectos...

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
