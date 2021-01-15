#pragma once

#ifndef _VOLUME_H_
#define _VOLUME_H_

#include <iostream>
#include <vector>
#include <string>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

template<typename Dtype>
Dtype MIN2(Dtype x, Dtype y) {
	return x > y ? y : x;
}

template<typename Dtype>
Dtype MAX2(Dtype x, Dtype y) {
	return x < y ? y : x;
}

template<typename Dtype>
bool inBall(Dtype center_x, Dtype center_y, Dtype center_z, Dtype radius,
			Dtype x, Dtype y, Dtype z) {
	return (x - center_x) * (x - center_x) + (y - center_y) * (y - center_y) +
		(z - center_z) * (z - center_z) > radius * radius ? false : true;
}

template<typename Dtype>
bool inCylinder(Dtype center_x, Dtype center_y, Dtype center_z, Dtype radius,
				Dtype length, Dtype x, Dtype y, Dtype z) {
	bool result = ((x - center_x) * (x - center_x) + (y - center_y) * (y - center_y) <=
		radius * radius) && (abs(z - center_z) <= length / 2);
	return result;
}

template<typename Dtype>
class Volume {
private:
	int volume_h;
	int volume_w;
	int volume_d;
	glm::ivec3 VoxelSize;
	vector<Dtype> _data;
public:

	Volume();
	~Volume();

	glm::ivec3 getVoxelSize();

	void Init();
	void LoadFromFlie(const char* filename, int w, int h, int d);
	vector<Dtype> getData();
};

template<typename Dtype>
Volume<Dtype>::Volume()
{
	Init();
}

template<typename Dtype>
Volume<Dtype>::~Volume()
{
	this->_data.clear();
}

template<typename Dtype>
glm::ivec3 Volume<Dtype>::getVoxelSize()
{
	return glm::ivec3(this->volume_w, this->volume_h, this->volume_d);
}

template<typename Dtype>
void Volume<Dtype>::Init()
{
	// cube shape with size (size_d x size_d x size_d), default size_d = 256
	const int size_d = 256;
	this->volume_d = size_d;
	this->volume_h = size_d;
	this->volume_w = size_d;
	int totalSize = this->volume_d * this->volume_h * this->volume_w;
	this->_data.resize(totalSize);

	glm::ivec3 center(this->volume_w / 2, this->volume_h / 2, this->volume_d / 2);
	int radius_ball = 100;
	int radius_cylinder = 50;
	int length_cylinder = 2 * int(sqrtf(radius_ball * radius_ball - radius_cylinder * radius_cylinder));

	for (int i = 0; i < this->volume_w; ++i) {
		for (int j = 0; j < this->volume_h; ++j) {
			for (int k = 0; k < this->volume_d; ++k) {
				bool isInBall = inBall(center.x, center.y, center.z, radius_ball, i, j, k);
				bool isInCylinder = inCylinder(center.x, center.y, center.z,
					radius_cylinder, length_cylinder, i, j, k);
				if (isInBall)
					this->_data[i * this->volume_d * this->volume_w + j * this->volume_d + k] = (Dtype)100;
				if (isInCylinder)
					this->_data[i * this->volume_d * this->volume_w + j * this->volume_d + k] = (Dtype)200;
			}
		}
	}
}

template<typename Dtype>
void Volume<Dtype>::LoadFromFlie(const char* filename, int w, int h, int d)
{

}

template<typename Dtype>
vector<Dtype> Volume<Dtype>::getData()
{
	return vector<Dtype>(this->_data);
}

#endif // _VOLUME_H_
