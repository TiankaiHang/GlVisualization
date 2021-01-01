#pragma once

#ifndef _VOLUME_H_
#define _VOLUME_H_

#include <iostream>
#include <vector>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

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
};

#endif // _VOLUME_H_
