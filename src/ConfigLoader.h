#pragma once

#include "Definitions.h"
#include "Light.h"
#include "Object.h"
#include "Sphere.h"
#include "Polygon.h"

namespace ConfigLoader {

	std::vector<std::string> explode(const std::string &s, char delim);
	void loadSceneNFF(const char* fileName, RGB * background, std::vector<Light> * lstLights, std::vector<Object*> * lstObjects, Viewpoint * camera);
	
}