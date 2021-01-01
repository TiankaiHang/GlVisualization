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
public:

	Volume();
	~Volume();

	void Init();
	void LoadFromFlie(string filename);
};

#endif // _VOLUME_H_
