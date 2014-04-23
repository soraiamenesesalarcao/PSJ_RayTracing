#include "ConfigLoader.h"

namespace ConfigLoader {

    // Split string
	std::vector<std::string> explode(const std::string &s, char delim){
		std::vector<std::string> result;
		std::istringstream iss (s);
		for (std::string token; std::getline(iss, token, delim);)
			result.push_back(token);
		return result;
	}
    
	void loadSceneNFF(const char* fileName, RGB * background, std::vector<Light> * lstLights, std::vector<Object*> * lstObjects, Viewpoint * camera){
		std::string line = std::string();
        std::ifstream file (fileName);
        bool flagBeforeObjects = true;
        int cylinderOrCone = 0;
        int vert_polygon = 0;
        int vert_polygon_patch = 0;

		RGB color;
		Material material;
		Vertex v = Vertex();
		std::vector<Vertex> vertices;

        if (file.is_open()) {
            while (getline(file, line)) {
                std::vector<std::string> splitedLine = explode(line, ' ');
                if(splitedLine.size() > 0) {
                    
                    // Comment
                    if(splitedLine[0] == "#") continue;
                    
                    // Viewpoint Location
                    else if(splitedLine[0] == "from" && flagBeforeObjects) {
						camera->from.px = atof(splitedLine[1].c_str());
						camera->from.py = atof(splitedLine[2].c_str());
						camera->from.pz = atof(splitedLine[3].c_str());
                    }
                    else if(splitedLine[0] == "at" && flagBeforeObjects) {
						camera->at.px = atof(splitedLine[1].c_str());
						camera->at.py = atof(splitedLine[2].c_str());
						camera->at.pz = atof(splitedLine[3].c_str());
					}
                    else if(splitedLine[0] == "up" && flagBeforeObjects) {
						camera->up.px = atof(splitedLine[1].c_str());
						camera->up.py = atof(splitedLine[2].c_str());
						camera->up.pz = atof(splitedLine[3].c_str());
					}
                    else if(splitedLine[0] == "angle" && flagBeforeObjects) {
						camera->angle = atof(splitedLine[1].c_str());
                    }
                    else if(splitedLine[0] == "hither" && flagBeforeObjects) {
						camera->hither = atof(splitedLine[1].c_str());
                    }
                    else if(splitedLine[0] == "resolution" && flagBeforeObjects) {
						camera->res = std::vector<int>(2);
						camera->res[0] = atoi(splitedLine[1].c_str());
						camera->res[1] = atoi(splitedLine[2].c_str());
					}
                    
                    // Background Color
                    else if(splitedLine[0] == "b" && flagBeforeObjects) {
						background->r = atof(splitedLine[1].c_str());
						background->g = atof(splitedLine[2].c_str());
						background->b = atof(splitedLine[3].c_str());
					}
                    
                    // Positional Light
                    else if(splitedLine[0] == "l" && flagBeforeObjects) {
						glm::vec3 light;
                        if (splitedLine.size() == 3) {
							light = glm::vec3(atof(splitedLine[1].c_str()), atof(splitedLine[2].c_str()), atof(splitedLine[3].c_str()));
							// default
							color.r = 0.5f;
							color.g = 0.5f;
							color.b = 0.5f;
                        }
                        else {
							light = glm::vec3(atof(splitedLine[1].c_str()), atof(splitedLine[2].c_str()), atof(splitedLine[3].c_str()));

							color.r = atof(splitedLine[4].c_str());
							color.g = atof(splitedLine[5].c_str());
							color.b = atof(splitedLine[6].c_str());
						}
						lstLights->push_back(Light(light,color));
                    }
                    
                    // Color and Shading Parameters
                    else if(splitedLine[0] == "f") {
						flagBeforeObjects = false;
						
						color.r = atof(splitedLine[1].c_str());
						color.g = atof(splitedLine[2].c_str());
						color.b = atof(splitedLine[3].c_str());
						
						material = Material(color, atof(splitedLine[4].c_str()), atof(splitedLine[5].c_str()), atof(splitedLine[6].c_str()),
										atof(splitedLine[7].c_str()), atof(splitedLine[8].c_str()));
					}
                    
                    // Cylinder or Cone
                    else if(splitedLine[0] == "c"){
                        cylinderOrCone = 1;
                        continue;
                    }/*
                    else if(cylinderOrCone == 1){ // first line
						cc.base_position.px = atof(splitedLine[1].c_str());
						cc.base_position.py = atof(splitedLine[2].c_str());
						cc.base_position.pz = atof(splitedLine[3].c_str());
						cc.base_radius = atof(splitedLine[4].c_str());
						cylinderOrCone++;
                    }
                    else if(cylinderOrCone == 2){ // second line
						cc.apex_position.px = atof(splitedLine[1].c_str());
						cc.apex_position.py = atof(splitedLine[2].c_str());
						cc.apex_position.pz = atof(splitedLine[3].c_str());
						cc.apex_radius = atof(splitedLine[4].c_str());
						cc.mtl = m;
                        cylinderOrCone = 0;
						scene1->coneAndCylinders.push_back(cc);
                    }*/
                    
                    // Sphere
                    else if(splitedLine[0] == "s"){
						glm::vec3 center = glm::vec3(atof(splitedLine[1].c_str()), atof(splitedLine[2].c_str()), atof(splitedLine[3].c_str()));
						float radius = atof(splitedLine[4].c_str());
						
						lstObjects->push_back(new Sphere(center, radius, material));
                    }

                    // Polygon
                    else if(splitedLine[0] == "p"){
                        vert_polygon = atof(splitedLine[1].c_str());
						vertices.clear();
                        continue;
                    }
                    else if(vert_polygon > 0){
						v.vx = atof(splitedLine[0].c_str());
						v.vy = atof(splitedLine[1].c_str());
						v.vz = atof(splitedLine[2].c_str());
						
						vertices.push_back(v);
						
						vert_polygon--;
						if(vert_polygon == 0) {
							lstObjects->push_back(new Polygon(vertices, material));

						}
                    }
                        
                    // Polygonal Patch
                    else if(splitedLine[0] == "pp"){
                        vert_polygon_patch = atof(splitedLine[1].c_str());
						vertices.clear();
                        continue;
                    }
                    else if(vert_polygon_patch > 0){
						v.vx = atof(splitedLine[0].c_str());
						v.vy = atof(splitedLine[1].c_str());
						v.vz = atof(splitedLine[2].c_str());
						vertices.push_back(v);
						
						vert_polygon_patch--;
						if(vert_polygon_patch == 0) {
							lstObjects->push_back(new Polygon(vertices, material));
						}
                    }
                }
            }
        }
        else {
			std::cerr << "Cannot open " << fileName << std::endl; 
			exit(1);
		}
	}
}