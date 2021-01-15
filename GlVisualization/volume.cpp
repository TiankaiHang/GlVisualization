//#include "volume.h"

//template<typename Dtype>
//Volume<Dtype>::Volume()
//{
//	Init();
//}
//
//template<typename Dtype>
//Volume<Dtype>::~Volume()
//{
//	this->_data.clear();
//}
//
//template<typename Dtype>
//glm::ivec3 Volume<Dtype>::getVoxelSize()
//{
//	return glm::ivec3(this->volume_w, this->volume_h, this->volume_d);
//}
//
//template<typename Dtype>
//void Volume<Dtype>::Init()
//{
//	// cube shape with size (size_d x size_d x size_d), default size_d = 256
//	const int size_d = 256;
//	this->volume_d = size_d;
//	this->volume_h = size_d;
//	this->volume_w = size_d;
//	int totalSize = this->volume_d * this->volume_h * this->volume_w;
//	this->_data.resize(totalSize);
//
//	glm::ivec3 center(this->volume_w / 2, this->volume_h / 2, this->volume_d / 2);
//	int radius_ball = 100;
//	int radius_cylinder = 50;
//	int length_cylinder = 2 * int(sqrtf(radius_ball * radius_ball - radius_cylinder * radius_cylinder));
//
//	for (int i = 0; i < this->volume_w; ++i) {
//		for (int j = 0; j < this->volume_h; ++j) {
//			for (int k = 0; k < this->volume_d; ++k) {
//				bool isInBall = inBall(center.x, center.y, center.z, radius_ball, i, j, k);
//				bool isInCylinder = inCylinder(center.x, center.y, center.z,
//					radius_cylinder, length_cylinder, i, j, k);
//				if (isInBall)
//					this->_data[i * this->volume_d * this->volume_w + j * this->volume_d + k] = (Dtype)100;
//				if(isInCylinder)
//					this->_data[i * this->volume_d * this->volume_w + j * this->volume_d + k] = (Dtype)200;
//			}
//		}
//	}
//}
//
//template<typename Dtype>
//void Volume<Dtype>::LoadFromFlie(const char* filename, int w, int h, int d)
//{
//
//}
//
//template<typename Dtype>
//vector<Dtype> Volume<Dtype>::getData()
//{
//	return vector<Dtype>(this->_data);
//}
