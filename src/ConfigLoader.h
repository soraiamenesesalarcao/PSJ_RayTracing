#pragma once

#include "Definitions.h"

namespace ConfigLoader {

	std::vector<std::string> explode(const std::string &s, char delim);
	void loadSceneNFF(const char* fileName, NFF * scene1);
	
}