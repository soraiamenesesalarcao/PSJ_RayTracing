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
	_ray.setX(x);
	_ray.setY(y);
	return _ray;
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




