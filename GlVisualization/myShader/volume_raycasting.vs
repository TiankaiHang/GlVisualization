// ref: https://github.com/toolchainX/Volume_Rendering_Using_GLSL/blob/master/shader/raycasting.vert

// #version 400
#version 330 core

layout (location = 0) in vec3 VerPos;
// have to use this variable!!!, or it will be very hard to debug for AMD video card
layout (location = 1) in vec3 VerClr;  


out vec3 EntryPoint;
out vec4 ExitPointCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    EntryPoint = VerPos;
    gl_Position = projection * view * vec4(vec3(model * vec4(VerPos, 1.0)), 1.0);
    ExitPointCoord = gl_Position;  
};