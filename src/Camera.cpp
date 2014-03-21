#include "Camera.h"



Camera::Camera() {	
	_winWidth = 640; // ir buscar ao NFF
	_winHeight = 480;	// ir buscar ao NFF
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

	distance = glm::length2(eye - at);	
	h = 2 * distance * glm::tan(fovy / 2.0);
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

void Camera::setResX(int w) {
	_winWidth = w;
}

void Camera::setResY(int h) {
	_winWidth = h;
}

Ray Camera::PrimaryRay(float x, float y) {
	return Ray();
}


void Camera::put() {	

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	
	glViewport(0, 0, _winWidth, _winHeight);
	gluPerspective(80.0f, ((float) _winWidth) / (float) _winHeight, 1, 5000 );	
	gluLookAt(	0.0, -1.0, 0.0, 	// eye
				0.0, 0.0, 0.0, 		// center
				0.0, 0.0, 1.0);		// up
			
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	        
}




