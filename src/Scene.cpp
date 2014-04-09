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
			//Ray ray = Camera::getInstance()->PrimaryRay(x, y);
			//RGB color = RayTracer::getInstance()->trace(_background, _lights, _objects, ray, 1, 1.0f);
			RGB color = monteCarlo(x, y, 1);
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
		glEnd(); //fica muito mais rapido!!!
		glFlush();
	}

	printf("Terminou!\n"); 
}

/* Monte Carlo Sampling!!! */
RGB Scene::monteCarlo(float x, float y, int depth){
	RGB color;

	glm::vec2 position[4];
	float margin = 1.0f / (2.0f * depth); 
	position[0] = glm::vec2(x+margin, y+margin); // top right
	position[1] = glm::vec2(x-margin, y+margin); // top left
	position[2] = glm::vec2(x-margin, y-margin); // bottom left
	position[3] = glm::vec2(x+margin, y-margin); // bottom right

	glm::vec3 colors[4];
	colors[0] = glm::vec3(0.0f, 0.0f, 0.0f);
	colors[1] = glm::vec3(0.0f, 0.0f, 0.0f);
	colors[2] = glm::vec3(0.0f, 0.0f, 0.0f);
	colors[3] = glm::vec3(0.0f, 0.0f, 0.0f);

	// It starts by tracing four rays at the corners of each pixel
	for(int i = 0; i < 4; i++){
		Ray ray = Camera::getInstance()->PrimaryRay(position[i].x, position[i].y);
		color = RayTracer::getInstance()->trace(_background, _lights, _objects, ray, 1, 1.0f);
		colors[i] = glm::vec3(color.r, color.g, color.b);
	}

	//If the colors are similar (check the threshold) then just use their average
	for(int i = 0, j = 1; i < 4 ; i++, j++){

		if(i == 3) j = 0;
		float diff = glm::abs(colors[i].r - colors[j].r) + glm::abs(colors[i].g - colors[j].g) + glm::abs(colors[i].b - colors[j].b);
		if( diff > THRESHOLD){ //different colors
			if(depth < 4){
				monteCarlo(position[i].x, position[i].y, depth+1);
			}
			break;
		}
	}

	color.r = color.g = color.b = 0.0f;
	for(int r = 0; r < 4; r++){
		color.r += colors[r].x;
		color.g += colors[r].y;
		color.b += colors[r].z;
	}

	color.r /= 4; color.g /= 4; color.b /= 4;

	return color;
}
