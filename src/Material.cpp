#include "Material.h"

Material::Material(){

}

Material::Material(RGB color, float kd, float ks, float shine, float t, float indexRefraction){
	_color = color;
	_kd = kd;
	_ks = ks;
	_shine = shine;
	_t = t;
	_indexRefraction = indexRefraction;
}

// GETS
float Material::getKd(){
	return _kd;
}

float Material::getKs(){
	return _ks;
}

float Material::getShine(){
	return _shine;
}

float Material::getT(){
	return _t;
}

float Material::getIndexRefraction(){
	return _indexRefraction;
}

RGB Material::getColor(){
	return _color;
}

// SETS
void Material::setKd(float kd){
	_kd = kd;
}

void Material::setKs(float ks){
	_ks = ks;
}

void Material::setShine(float shine){
	_shine = shine;
}

void Material::setT(float t){
	_t = t;
}

void Material::setIndexRefraction(float indexRefraction){
	_indexRefraction = indexRefraction;
}

void Material::setColor(RGB color){
	_color = color;
}