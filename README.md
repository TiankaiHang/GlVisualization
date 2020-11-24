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

<img src="https://github.com/TiankaiHang/GlVisualization/blob/master/GlVisualization/figs/ball1.png" style="zoom:20%"/>

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