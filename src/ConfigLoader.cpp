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
    
	void loadSceneNFF(const char* fileName, NFF * scene1){
		std::string line = std::string();
        std::ifstream file (fileName);
        int flagBeforeObjects = true;
        int cylinderOrCone = 0;
        int vert_polygon = 0;
        int vert_polygon_patch = 0;

		Light l = Light();
		Material m = Material();
		ConeCylinder cc = ConeCylinder();
		Plan plan = Plan();
		Polygon polygon = Polygon();
		PolygonPatch polyPatch = PolygonPatch();
		Sphere sphere = Sphere();
		Vertex v = Vertex();
		Normal norm = Normal();
        
        if (file.is_open()) {
            while (getline(file, line)) {
                std::vector<std::string> splitedLine = explode(line, ' ');
                if(splitedLine.size() > 0) {
                    
                    // Comment
                    if(splitedLine[0] == "#") continue;
                    
                    // Viewpoint Location
                    else if(splitedLine[0] == "from" && flagBeforeObjects) {
						scene1->camera.from.px = atof(splitedLine[1].c_str());
						scene1->camera.from.py = atof(splitedLine[2].c_str());
						scene1->camera.from.pz = atof(splitedLine[3].c_str());
                    }
                    else if(splitedLine[0] == "at" && flagBeforeObjects) {
						scene1->camera.at.px = atof(splitedLine[1].c_str());
						scene1->camera.at.py = atof(splitedLine[2].c_str());
						scene1->camera.at.pz = atof(splitedLine[3].c_str());
					}
                    else if(splitedLine[0] == "up" && flagBeforeObjects) {
						scene1->camera.up.px = atof(splitedLine[1].c_str());
						scene1->camera.up.py = atof(splitedLine[2].c_str());
						scene1->camera.up.pz = atof(splitedLine[3].c_str());
					}
                    else if(splitedLine[0] == "angle" && flagBeforeObjects) {
						scene1->camera.angle = atof(splitedLine[1].c_str());
                    }
                    else if(splitedLine[0] == "hither" && flagBeforeObjects) {
						scene1->camera.hither = atof(splitedLine[1].c_str());
                    }
                    else if(splitedLine[0] == "resolution" && flagBeforeObjects) {
						scene1->camera.res = std::vector<int>(2);
						scene1->camera.res[0] = atoi(splitedLine[1].c_str());
						scene1->camera.res[1] = atoi(splitedLine[1].c_str());
					}
                    
                    // Background Color
                    else if(splitedLine[0] == "b" && flagBeforeObjects) {
						scene1->background.r = atof(splitedLine[1].c_str());
						scene1->background.b = atof(splitedLine[2].c_str());
						scene1->background.g = atof(splitedLine[3].c_str());
					}
                    
                    // Positional Light
                    else if(splitedLine[0] == "l" && flagBeforeObjects) {

                        if (splitedLine.size() == 3) {
							l.position.px = atof(splitedLine[1].c_str());
							l.position.py = atof(splitedLine[2].c_str());
							l.position.pz = atof(splitedLine[3].c_str());	
                        }
                        else {
							l.position.px = atof(splitedLine[1].c_str());
							l.position.py = atof(splitedLine[2].c_str());
							l.position.pz = atof(splitedLine[3].c_str());
							l.color.r = atof(splitedLine[4].c_str());
							l.color.g = atof(splitedLine[5].c_str());
							l.color.b = atof(splitedLine[6].c_str());
						}
						scene1->lights.push_back(l);
                    }
                    
                    // Color and Shading Parameters
                    else if(splitedLine[0] == "f") {
						flagBeforeObjects = false;
						
						m.color.r = atof(splitedLine[1].c_str());
						m.color.g = atof(splitedLine[2].c_str());
						m.color.b = atof(splitedLine[3].c_str());
						m.kd = atof(splitedLine[4].c_str());
						m.ks = atof(splitedLine[5].c_str());
						m.shine = atof(splitedLine[6].c_str());
						m.t = atof(splitedLine[7].c_str());
						m.indexRefraction = atof(splitedLine[8].c_str());	
					}
                    
                    // Cylinder or Cone
                    else if(splitedLine[0] == "c"){
                        std::cout << "Cylinder or Cone" << std::endl;
                        cylinderOrCone = 1;
                        continue;
                    }
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
                    }
                    
                    // Sphere
                    else if(splitedLine[0] == "s"){
						sphere.center.px = atof(splitedLine[1].c_str());
						sphere.center.py = atof(splitedLine[2].c_str());
						sphere.center.pz = atof(splitedLine[3].c_str());
						sphere.radius = atof(splitedLine[4].c_str());
						sphere.mtl = m;
						scene1->spheres.push_back(sphere);
                    }
                    
                    // Plane
                    else if(splitedLine[0] == "pl"){
						plan.point_1.px = atof(splitedLine[1].c_str());
						plan.point_1.py = atof(splitedLine[2].c_str());
						plan.point_1.pz = atof(splitedLine[3].c_str());
						plan.point_2.px = atof(splitedLine[4].c_str());
						plan.point_2.py = atof(splitedLine[5].c_str());
						plan.point_2.pz = atof(splitedLine[6].c_str());
						plan.point_3.px = atof(splitedLine[7].c_str());
						plan.point_3.py = atof(splitedLine[8].c_str());
						plan.point_3.pz = atof(splitedLine[9].c_str());
						plan.mtl = m;
						scene1->planes.push_back(plan);
                    }

                    // Polygon
                    else if(splitedLine[0] == "p"){
                        std::cout << "Polygon" << std::endl;
                        vert_polygon = atof(splitedLine[1].c_str());
                        continue;
                    }
                    else if(vert_polygon > 0){
						v.vx = atof(splitedLine[1].c_str());
						v.vy = atof(splitedLine[2].c_str());
						v.vz = atof(splitedLine[3].c_str());
						polygon.vertices.push_back(v);

						vert_polygon--;
						if(vert_polygon == 0) {
							polygon.mtl = m;
							scene1->polygons.push_back(polygon);
						}
                    }
                        
                    // Polygonal Patch
                    else if(splitedLine[0] == "pp"){
                        std::cout << "Polygon Patch" << std::endl;
                        vert_polygon_patch = atof(splitedLine[1].c_str());
                        continue;
                    }
                    else if(vert_polygon_patch > 0){
						v.vx = atof(splitedLine[1].c_str());
						v.vy = atof(splitedLine[2].c_str());
						v.vz = atof(splitedLine[3].c_str());
						polyPatch.vertices.push_back(v);

						norm.nx = atof(splitedLine[4].c_str());
						norm.ny = atof(splitedLine[5].c_str());
						norm.nz = atof(splitedLine[6].c_str());
						polyPatch.normals.push_back(norm);
						
						vert_polygon_patch--;
						if(vert_polygon_patch == 0) {
							polyPatch.mtl = m;
							scene1->polygonPatchs.push_back(polyPatch);
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