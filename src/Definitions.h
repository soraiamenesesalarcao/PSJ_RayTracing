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

#include <omp.h>

// Macros
#define MAX_DEPTH 6
#define EPSILON 0.001f
#define LIGHT_SIDE 4
#define LIGHT_GRID_RATIO (1.0 / LIGHT_SIDE)
#define LIGHT_EPSILON 0.1
#define THRESHOLD 0.3f
#define MULTIPLY_FACTOR 2

#define FOCAL_LENGHT 2.5f
#define DEPTH_RAYS 4

// Vertex of a polygon
typedef struct {
    float vx;
    float vy;
    float vz;
} Vertex;

// Position
typedef struct {
    float px;
    float py;
    float pz;
} Position;

// Color components
typedef struct {
    float r;
    float g;
    float b;
} RGB;

// Camera info
typedef struct {
	Position from;
    Position at;
	Position up;
    float angle;
    float hither;
    std::vector<int> res;
} Viewpoint;