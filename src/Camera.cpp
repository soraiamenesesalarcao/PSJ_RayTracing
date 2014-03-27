#include "Camera.h"



Camera::Camera() {	
	_winWidth = 640; 
	_winHeight = 480;
}

Camera::~Camera() {
}

Camera * Camera::getInstance(){
	static Camera instance;
	return &instance;
}

void Camera::init(Viewpoint * viewpoint){
	_winWidth = viewpoint->res[0];
	_winHeight = viewpoint->res[1];
	eye = glm::vec3(viewpoint->from.px, viewpoint->from.py, viewpoint->from.pz);
	at = glm::vec3(viewpoint->at.px, viewpoint->at.py, viewpoint->at.pz);
	up = glm::vec3(viewpoint->up.px, viewpoint->up.py, viewpoint->up.pz);
	fovy = glm::radians(viewpoint->angle); 
	near = viewpoint->hither; //hither	

	df = glm::length(eye - at); //distance
	h = 2 * df * glm::tan(fovy / 2.0);
	w = h * (_winWidth / _winHeight);	

	ze = glm::normalize(eye - at);
	xe = glm::normalize(glm::cross(up, ze));
	ye = glm::cross(ze, xe);
}

int Camera::GetResX() {
	return _winWidth;
}

int Camera::GetResY(){
	return _winHeight;
}

glm::vec3 Camera::computeV() {
	return eye - at;
}

void Camera::setResX(int w) {
	_winWidth = w;
}

void Camera::setResY(int h) {
	_winWidth = h;
}

/* Calcula o raio primario
 * Recebe uma posicao de um pixel do viewport
 *
 * Tem como origem o valor do eye
 * A direcao é calculada a partir de (de acordo com os slides):
 * u(x) = (x/ResX-1)w
 * v(y) = (y/ResY-1)h
 * direction = -df*ze + h((y/ResY-1) - 1/2)ye + w((x/ResX-1) - 1/2)xe =
 *			 = -df*ze + (v(y) - h/2)ye + (u(x) + w/2)xe
 * Direction tem que ser normalizada porque é uma direccao
 */
Ray Camera::PrimaryRay(float x, float y) {
	Ray ray;
	float u_x = (x/(_winWidth-1)) * w;
	float v_y = (y/(_winHeight-1)) * h;

	ray.origin = eye;
	ray.direction = glm::normalize(-df*ze + (v_y - h/2)*ye + (u_x - w/2)*xe); //it's a direction so don't forget to normalize

	return ray;
}


void Camera::put() {	

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	
	glViewport(0, 0, _winWidth, _winHeight);
	gluPerspective(fovy, ((float) _winWidth) / (float) _winHeight, 0.01, 100 );	
	gluLookAt(	eye.x, eye.y, eye.z, 	// eye
				at.x, at.y, at.z, 		// center
				up.x, up.y, up.z);		// up
			
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	        
}




