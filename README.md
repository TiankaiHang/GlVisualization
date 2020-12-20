# Code for Visualization Course

Based on Visual Studio 2019, GLFW and GLAD.

GLFW: https://www.glfw.org/download.html, compile the source code by CMake to generate the code.

GLAD: https://glad.dav1d.de/

## Draw Some Shapes

### Some demo

#### ball

500*500 vertices

<img src="https://github.com/TiankaiHang/GlVisualization/blob/master/GlVisualization/figs/ball.png" style="zoom:20%"/>

500*500 vertices

<img width=240 height=180 src="https://github.com/TiankaiHang/GlVisualization/blob/master/GlVisualization/figs/ball1.png" style="zoom:20%"/>

50*50 vertices

<img src="https://github.com/TiankaiHang/GlVisualization/blob/master/GlVisualization/figs/ball2.png" style="zoom:20%"/>

#### Cylinder

```c
const float CYLINDER_RADIUS = 0.5;
const float CYLINDER_HEIGHT = 1.0;
const int CIRCLE_FACE_ANGLE_INTERVALS = 100;
const int CIRCLE_FACE_RADIUS_INTERVALS = 100;
    
const float CIRCLE_FACE_ANGLE_DELTA = 2.0 * PI / CIRCLE_FACE_ANGLE_INTERVALS;
const float CIRCLE_FACE_RADIUS_DELTA = CYLINDER_RADIUS / CIRCLE_FACE_RADIUS_INTERVALS;

const int SIDE_FACE_INTERVALS_X = 100;
const int SIDE_FACE_INTERVALS_Y = 100;
const float SIDE_FACE_ANGLE_DELTA = 2.0 * PI / SIDE_FACE_INTERVALS_X;
const float SIDE_FACE_HEIGHT_DELTA = CYLINDER_HEIGHT / SIDE_FACE_INTERVALS_Y;
    
vector<float> NORMAL = { 0.0, 1.0, 0.0 };
float ROTATE_ANGLE = 60.0 * PI / 180.0;
vector<float> OFFSET = { 0.0, 0.0, 0.0 };
```

<img src="https://github.com/TiankaiHang/GlVisualization/blob/master/GlVisualization/figs/cylinder_60.png" style="zoom:20%"/>

```c
const float CYLINDER_RADIUS = 0.5;
const float CYLINDER_HEIGHT = 1.0;
const int CIRCLE_FACE_ANGLE_INTERVALS = 100;
const int CIRCLE_FACE_RADIUS_INTERVALS = 100;
    
const float CIRCLE_FACE_ANGLE_DELTA = 2.0 * PI / CIRCLE_FACE_ANGLE_INTERVALS;
const float CIRCLE_FACE_RADIUS_DELTA = CYLINDER_RADIUS / CIRCLE_FACE_RADIUS_INTERVALS;

const int SIDE_FACE_INTERVALS_X = 100;
const int SIDE_FACE_INTERVALS_Y = 100;
const float SIDE_FACE_ANGLE_DELTA = 2.0 * PI / SIDE_FACE_INTERVALS_X;
const float SIDE_FACE_HEIGHT_DELTA = CYLINDER_HEIGHT / SIDE_FACE_INTERVALS_Y;
    
vector<float> NORMAL = { 1.0, 1.0, 1.0 };
float ROTATE_ANGLE = 60.0 * PI / 180.0;
vector<float> OFFSET = { 0.0, 0.0, 0.0 };
```

<img src="https://github.com/TiankaiHang/GlVisualization/blob/master/GlVisualization/figs/cylinder_norm111.png" style="zoom:20%"/>

with different colors illustrating

<img src="https://github.com/TiankaiHang/GlVisualization/blob/master/GlVisualization/figs/cylinder_norm111_diff_color.png" style="zoom:20%"/>

<img src="https://github.com/TiankaiHang/GlVisualization/blob/master/GlVisualization/figs/cylinder_norm111_diff_color_ploygon.png" style="zoom:20%"/>

## 具体实验内容

### 实验1

- 熟悉OpenGL使用
- 显示一个球面和正方形表面
  - 数据结构、颜色（RGBA）、视见变换（改变观察点观察正方形显示）
  - 光照模型（改变镜面反射参数观察球面显示效果）
- 实验报告

### 实验2

- 实现体绘制的光线跟踪算法
- 实验与综述报告（不需要单独实验报告）

### 综述报告

- 体积绘制方法综述报告
- 研究背景、方法原理、算法、实验讨论、结论、参考文献

### 时间安排

- 学期结束前提交：source code、实验报告、综述报告
- 研究生网站下载评语页注明、填写相应内容
- 文件打包，“学号姓名”命名，发送到 luo.list@seu.edu.cn



