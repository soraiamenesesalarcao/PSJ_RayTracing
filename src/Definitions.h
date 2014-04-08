#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <stdio.h> 
#include <cmath>
#include <algorithm>

#include <GL/glut.h>

#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/intersect.hpp>
#include <gtx/quaternion.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/transform.hpp>
#include <gtx/transform2.hpp>
#include <gtc/matrix_transform.hpp>
#include <ext.hpp>

// MACROS
#define MAX_DEPTH 6
#define EPSILON 0.001f
#define SHADOW_RAYS 25

// Structure for representing a vertex of a polygon
typedef struct {
    float vx;
    float vy;
    float vz;
} Vertex;

// Structure for representing the normal of a polygon
typedef struct {
    float nx;
    float ny;
    float nz;
} Normal;

//NAO DEVE SER NECESSARIO
typedef struct {
    float px;
    float py;
    float pz;
} Position;

typedef struct {
    float r;
    float g;
    float b;
} RGB;

// Structure for the info of the camera
typedef struct {
	Position from;
    Position at;
	Position up;
    float angle;
    float hither;
    std::vector<int> res;
} Viewpoint;

// Structure for the info of each cone or cylinder
typedef struct {
	Position base_position;
	float base_radius;
	Position apex_position;
	float apex_radius;

	//Material mtl;
} ConeCylinder;


// Structure for the info of each polygon patch
typedef struct {
    int num_vertices;
    std::vector<Vertex> vertices;
    std::vector<Normal> normals;
    //Material mtl;
} PolygonPatch;

// Structure for the info of the nff file
typedef struct {
    std::vector<ConeCylinder> coneAndCylinders;
    std::vector<PolygonPatch> polygonPatchs;
} NFF;