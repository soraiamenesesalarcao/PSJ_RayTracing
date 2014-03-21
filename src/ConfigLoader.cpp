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
    
	void loadSceneNFF(const char* fileName, NFF scene1){
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
                        std::cout << "Viewpoint - From" << std::endl;
                        std::cout << "fx: " << splitedLine[1] << " fx: " << splitedLine[2] << " fx: " << splitedLine[3] << std::endl;
						scene1.camera.from.px = atof(splitedLine[1].c_str());
						scene1.camera.from.py = atof(splitedLine[2].c_str());
						scene1.camera.from.pz = atof(splitedLine[3].c_str());
						 std::cout << "fx: " << scene1.camera.from.px << " fx: " << scene1.camera.from.py << " fx: " << scene1.camera.from.pz << std::endl;
                    }
                    else if(splitedLine[0] == "at" && flagBeforeObjects) {
                        std::cout << "Viewpoint - At" << std::endl;
                        std::cout << "ax: " << splitedLine[1] << " ax: " << splitedLine[2] << " ax: " << splitedLine[3] << std::endl;
						scene1.camera.at.px = atof(splitedLine[1].c_str());
						scene1.camera.at.py = atof(splitedLine[2].c_str());
						scene1.camera.at.pz = atof(splitedLine[3].c_str());
						std::cout << "ax: " << scene1.camera.at.px << " ax: " << scene1.camera.at.py << " ax: " << scene1.camera.at.pz << std::endl;
                    }
                    else if(splitedLine[0] == "up" && flagBeforeObjects) {
                        std::cout << "Viewpoint - Up" << std::endl;
                        std::cout << "ux: " << splitedLine[1] << " ux: " << splitedLine[2] << " ux: " << splitedLine[3] << std::endl;
						scene1.camera.up.px = atof(splitedLine[1].c_str());
						scene1.camera.up.py = atof(splitedLine[2].c_str());
						scene1.camera.up.pz = atof(splitedLine[3].c_str());
						std::cout << "ux: " << scene1.camera.up.px << " ux: " << scene1.camera.up.py << " ux: " << scene1.camera.up.pz << std::endl;
                    }
                    else if(splitedLine[0] == "angle" && flagBeforeObjects) {
                        std::cout << "Viewpoint - Angle" << std::endl;
                        std::cout << "ang: " << splitedLine[1] << std::endl;
						scene1.camera.angle = atof(splitedLine[1].c_str());
						std::cout << "ang: " << scene1.camera.angle << std::endl;
                    }
                    else if(splitedLine[0] == "hither" && flagBeforeObjects) {
                        std::cout << "Viewpoint - Hither" << std::endl;
                        std::cout << "hit: " << splitedLine[1] << std::endl;
						scene1.camera.hither = atof(splitedLine[1].c_str());
						std::cout << "hit: " << scene1.camera.hither << std::endl;
                    }
                    else if(splitedLine[0] == "resolution" && flagBeforeObjects) {
						scene1.camera.res = std::vector<int>(2);
                        std::cout << "Viewpoint - Resolution" << std::endl;
                        std::cout << "resx: " << splitedLine[1] << " resy: " << splitedLine[2] << std::endl;
						scene1.camera.res[0] = atoi(splitedLine[1].c_str());
						scene1.camera.res[1] = atoi(splitedLine[1].c_str());
						std::cout << "resx: " << scene1.camera.res[0] << " resy: " << scene1.camera.res[1]  << std::endl;
                    }
                    
                    // Background Color
                    else if(splitedLine[0] == "b" && flagBeforeObjects) {
                        std::cout << "Background Color" << std::endl;
                        std::cout << "r: " << splitedLine[1] << " g: " << splitedLine[2] << " b: " << splitedLine[3] << std::endl;
						scene1.background.r = atof(splitedLine[1].c_str());
						scene1.background.b = atof(splitedLine[2].c_str());
						scene1.background.g = atof(splitedLine[3].c_str());
						std::cout << "r: " << scene1.background.r << " g: " << scene1.background.b << " b: " << scene1.background.g << std::endl;
                    }
                    
                    // Positional Light
                    else if(splitedLine[0] == "l" && flagBeforeObjects) {
						
                        std::cout << "Positional Light" << std::endl;
                        if (splitedLine.size() == 3) {
                            std::cout << "x: " << splitedLine[1] << " y: " << splitedLine[2] << " z: " << splitedLine[3] << std::endl;							
							l.position.px = atof(splitedLine[1].c_str());
							l.position.py = atof(splitedLine[2].c_str());
							l.position.pz = atof(splitedLine[3].c_str());
							std::cout << "x: " << l.position.px << " y: " << l.position.py  << " z: " << l.position.pz << std::endl;
							
                        }
                        else {
                            std::cout << "x: " << splitedLine[1] << " y: " << splitedLine[2] << " z: " << splitedLine[3]
                                      << " r: " << splitedLine[4] << " g: " << splitedLine[5] << " b: " << splitedLine[6] << std::endl;
							l.position.px = atof(splitedLine[1].c_str());
							l.position.py = atof(splitedLine[2].c_str());
							l.position.pz = atof(splitedLine[3].c_str());

							l.color.r = atof(splitedLine[4].c_str());
							l.color.g = atof(splitedLine[5].c_str());
							l.color.b = atof(splitedLine[6].c_str());

							std::cout << "x: " << l.position.px << " y: " << l.position.py  << " z: " << l.position.pz
								<< " r: " << l.color.r << " g: " << l.color.g << " b: " << l.color.b << std::endl;
                        }
						scene1.lights.push_back(l);
                    }
                    
                    // Color and Shading Parameters
                    else if(splitedLine[0] == "f") {
                        std::cout << "Color and Shading Parameters" << std::endl;
                        flagBeforeObjects = false;
						std::cout << "r: " << splitedLine[1] << " g: " << splitedLine[2] << " b: " << splitedLine[3]
                                  << " kd: " << splitedLine[4] << " ks: " << splitedLine[5] << " s: " << splitedLine[6]
                                  << " t: " << splitedLine[7] << " re: " << splitedLine[8] << std::endl;
								  m.color.r = atof(splitedLine[1].c_str());
								  m.color.g = atof(splitedLine[2].c_str());
								  m.color.b = atof(splitedLine[3].c_str());

								  m.kd = atof(splitedLine[4].c_str());
								  m.ks = atof(splitedLine[5].c_str());
								  m.shine = atof(splitedLine[6].c_str());
								  
								  m.t = atof(splitedLine[7].c_str());
								  m.indexRefraction = atof(splitedLine[8].c_str());	

						std::cout << "r: " << m.color.r << " g: " << m.color.g << " b: " << m.color.b
                                  << " kd: " << m.kd << " ks: " << m.ks << " s: " << m.shine
                                  << " t: " << m.t << " re: " << m.indexRefraction << std::endl;
                    }
                    
                    // Cylinder or Cone
                    else if(splitedLine[0] == "c"){
                        std::cout << "Cylinder or Cone" << std::endl;
                        cylinderOrCone = 1;
                        continue;
                    }
                    else if(cylinderOrCone == 1){ // first line
                        std::cout << "bx: " << splitedLine[1] << " by: " << splitedLine[2] << " bz: " << splitedLine[3]
                                  << " br: " << splitedLine[4] << std::endl;
						cc.base_position.px = atof(splitedLine[1].c_str());
						cc.base_position.py = atof(splitedLine[2].c_str());
						cc.base_position.pz = atof(splitedLine[3].c_str());
						cc.base_radius = atof(splitedLine[4].c_str());

						std::cout << "bx: " << cc.base_position.px << " by: " << cc.base_position.py << " bz: " << cc.base_position.pz
                                  << " br: " << cc.base_radius << std::endl;

                        cylinderOrCone++;
                    }
                    else if(cylinderOrCone == 2){ // second line
                        std::cout << "ax: " << splitedLine[1] << " ay: " << splitedLine[2] << " az: " << splitedLine[3]
                                  << " ar: " << splitedLine[4] << std::endl;

						cc.apex_position.px = atof(splitedLine[1].c_str());
						cc.apex_position.py = atof(splitedLine[2].c_str());
						cc.apex_position.pz = atof(splitedLine[3].c_str());
						cc.apex_radius = atof(splitedLine[4].c_str());

						std::cout << "ax: " << cc.apex_position.px << " ay: " << cc.apex_position.py << " az: " << cc.apex_position.pz
                                  << " ar: " << cc.apex_radius << std::endl;

						cc.mtl = m;
                        cylinderOrCone = 0;
						scene1.coneAndCylinders.push_back(cc);
                    }
                    
                    // Sphere
                    else if(splitedLine[0] == "s"){
                        std::cout << "Sphere" << std::endl;
                        std::cout << "cx: " << splitedLine[1] << " cy: " << splitedLine[2] << " cz: " << splitedLine[3]
                                  << " r: " << splitedLine[4] << std::endl;

						sphere.center.px = atof(splitedLine[1].c_str());
						sphere.center.py = atof(splitedLine[2].c_str());
						sphere.center.pz = atof(splitedLine[3].c_str());
						sphere.radius = atof(splitedLine[4].c_str());

						std::cout << "Sphere" << std::endl;
                        std::cout << "cx: " << sphere.center.px << " cy: " << sphere.center.py << " cz: " << sphere.center.pz
                                  << " r: " << sphere.radius << std::endl;

						sphere.mtl = m;
						scene1.spheres.push_back(sphere);
                    }
                    
                    // Plane
                    else if(splitedLine[0] == "pl"){
                        std::cout << "Plane" << std::endl;
                        std::cout << "p1x: " << splitedLine[1] << " p1y: " << splitedLine[2] << " p1z: " << splitedLine[3]
                                  << " p2x: " << splitedLine[4] << " p2y: " << splitedLine[5] << " p2z: " << splitedLine[6]
                                  << " p3x: " << splitedLine[7] << " p3y: " << splitedLine[8] << " p3z: " << splitedLine[9] << std::endl;

						plan.point_1.px = atof(splitedLine[1].c_str());
						plan.point_1.py = atof(splitedLine[2].c_str());
						plan.point_1.pz = atof(splitedLine[3].c_str());
						plan.point_2.px = atof(splitedLine[4].c_str());
						plan.point_2.py = atof(splitedLine[5].c_str());
						plan.point_2.pz = atof(splitedLine[6].c_str());
						plan.point_3.px = atof(splitedLine[7].c_str());
						plan.point_3.py = atof(splitedLine[8].c_str());
						plan.point_3.pz = atof(splitedLine[9].c_str());

						std::cout << "Plane" << std::endl;
                        std::cout << "p1x: " << splitedLine[1] << " p1y: " << splitedLine[2] << " p1z: " << splitedLine[3]
                                  << " p2x: " << splitedLine[4] << " p2y: " << splitedLine[5] << " p2z: " << splitedLine[6]
                                  << " p3x: " << splitedLine[7] << " p3y: " << splitedLine[8] << " p3z: " << splitedLine[9] << std::endl;
						plan.mtl = m;
                    }

                    // Polygon
                    else if(splitedLine[0] == "p"){
                        std::cout << "Polygon" << std::endl;
                        vert_polygon = atof(splitedLine[1].c_str());
                        continue;
                    }
                    else if(vert_polygon > 0){
                        std::cout << "x: " << splitedLine[1] << " y: " << splitedLine[2] << " z: " << splitedLine[3] << std::endl;

						v.vx = atof(splitedLine[1].c_str());
						v.vy = atof(splitedLine[2].c_str());
						v.vz = atof(splitedLine[3].c_str());
						polygon.vertices.push_back(v);

						std::cout << "x: " << v.vx << " y: " << v.vy << " z: " << v.vz << std::endl;

                        vert_polygon--;
						if(vert_polygon == 0) {
							polygon.mtl = m;
							scene1.polygons.push_back(polygon);
						}

                    }
                        
                    // Polygonal Patch
                    else if(splitedLine[0] == "pp"){
                        std::cout << "Polygon Patch" << std::endl;
                        vert_polygon_patch = atof(splitedLine[1].c_str());
                        continue;
                    }
                    else if(vert_polygon_patch > 0){
                        std::cout << "x: " << splitedLine[1] << " y: " << splitedLine[2] << " z: " << splitedLine[3] << std::endl;

						v.vx = atof(splitedLine[1].c_str());
						v.vy = atof(splitedLine[2].c_str());
						v.vz = atof(splitedLine[3].c_str());
						polyPatch.vertices.push_back(v);

						std::cout << "x: " << v.vx << " y: " << v.vy << " z: " << v.vz << std::endl;

                        vert_polygon_patch--;
						if(vert_polygon_patch == 0) {
							polyPatch.mtl = m;
							scene1.polygonPatchs.push_back(polyPatch);
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