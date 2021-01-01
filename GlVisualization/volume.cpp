#include "volume.h"

Volume::Volume()
{

}

Volume::~Volume()
{
}

glm::vec3 Volume::getVoxelSize()
{
	return glm::vec3(this->volume_w, this->volume_h, this->volume_d);
}

void Volume::Init()
{
	// cube shape with size (size_d x size_d x size_d), default size_d = 256
	const int size_d = 256;
	this->volume_d = size_d;
	this->volume_h = size_d;
	this->volume_w = size_d;
	int totalSize = this->volume_d * this->volume_h * this->volume_w;
	this->_data.resize(this->volume_d);
}
