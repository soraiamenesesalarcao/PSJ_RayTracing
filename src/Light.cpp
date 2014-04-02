#include "Light.h"

Light::Light(glm::vec3 position, RGB color){
	_position = position;
	_color = color;
}

glm::vec3 Light::getPosition(){
	return _position;
}

RGB Light::getColor(){
	return _color;
}

void Light::setPosition(float x, float y, float z){
	_position = glm::vec3(x,y,z);

}