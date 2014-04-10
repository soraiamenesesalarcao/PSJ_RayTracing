#include "Scene.h"

/* ************************************************************************** */
/* ********************************* GLOBALS ******************************** */
/* ************************************************************************** */

int RES_X = 500, RES_Y = 500;

/* ************************************************************************** */
/* ******************************* CALLBACKS ******************************** */
/* ************************************************************************** */


void reshape(int w, int h) { 
	glClearColor(0.0, 0.0, 0.0, 0.5);
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	gluOrtho2D(0, RES_X-1, 0, RES_Y-1);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity(); 
}


void drawScene() { 	
	Scene::getInstance()->init();
	Scene::getInstance()->draw();
	Scene::getInstance()->~Scene();
}


/* ************************************************************************** */
/* *********************************** SETUP ******************************** */
/* ************************************************************************** */

void setupCallbacks()  {
	glutReshapeFunc(reshape);
	glutDisplayFunc(drawScene);
}


void setupOpenGL() {
	glClearColor(0.0, 0.0, 0.0, 0.5);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
}


void setupGLUT(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(RES_X-1, RES_Y-1);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Assignment 3: Ray Tracing");
}


/* ************************************************************************** */
/* ************************************ INIT ******************************** */
/* ************************************************************************** */

void init(int argc, char* argv[]) {
	 // ordem relevante
	Scene::getInstance()->init();
	RES_X = Scene::getInstance()->getCamera().GetResX();
	RES_Y = Scene::getInstance()->getCamera().GetResY(); 
	printf("resx = %d resy= %d.\n", RES_X, RES_Y);

	setupGLUT(argc, argv);	
	setupOpenGL();	
	setupCallbacks();
}

/* ************************************************************************** */
/* ************************************ MAIN ******************************** */
/* ************************************************************************** */

int main(int argc, char**argv) { 
	
	init(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}