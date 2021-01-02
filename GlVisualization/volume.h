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

class Volume {
private:
	int volume_h;
	int volume_w;
	int volume_d;
	glm::vec3 VoxelSize;
	vector<int> _data;
public:

	Volume();
	~Volume();

	glm::vec3 getVoxelSize();

	void Init();
	void LoadFromFlie(const char* filename, int w, int h, int d);
	vector<int> getData();
};

#endif // _VOLUME_H_
