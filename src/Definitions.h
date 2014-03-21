#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <stdio.h> 

#include <GL/glut.h>

// MACROS
#define MAX_DEPTH 6

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

typedef struct{
    RGB color;
    float kd;
    float ks;
    float shine;
    float t;
    float indexRefraction;
} Material;

// Structure for the info of the camera
typedef struct {
    //std::vector<Position> from;
    //std::vector<Position> at;
    //std::vector<Position> up;
	Position from;
    Position at;
	Position up;
    float angle;
    float hither;
    std::vector<int> res;
} Viewpoint;

// Structure for the info of each light
typedef struct {
    //std::vector<Position> position;
	Position position;
    RGB color;
} Light;

// Structure for the info of each cone or cylinder
typedef struct {
    //std::vector<Position> base_position;
	Position base_position;
//    std::vector<float> base_radius;
	float base_radius;
 //   std::vector<Position> apex_position;
	Position apex_position;
  //  std::vector<float> apex_radius;
	float apex_radius;

	Material mtl;
} ConeCylinder;

// Structure for the info of each plan
typedef struct{
  /*  std::vector<Position> point_1;
    std::vector<Position> point_2;
    std::vector<Position> point_3;*/
	Position point_1;
    Position point_2;
    Position point_3;
    Material mtl;
} Plan;

// Structure for the info of each sphere
typedef struct {
    //std::vector<Position> center;
	Position center;
    float radius;
    Material mtl;
} Sphere;

// Structure for the info of each polygon
typedef struct {
    int num_vertices;
    std::vector<Vertex> vertices;
    Material mtl;
} Polygon;

// Structure for the info of each polygon patch
typedef struct {
    int num_vertices;
    std::vector<Vertex> vertices;
    std::vector<Normal> normals;
    Material mtl;
} PolygonPatch;

// Structure for the info of the nff file
typedef struct {
    RGB background;
    Viewpoint camera;
    std::vector<ConeCylinder> coneAndCylinders;
    std::vector<Light> lights;
    std::vector<Plan> planes;
    std::vector<Polygon> polygons;
    std::vector<PolygonPatch> polygonPatchs;
    std::vector<Sphere> spheres;
} NFF;
