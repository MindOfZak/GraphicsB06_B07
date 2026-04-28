#version 430 core

layout (location = 0) in vec4 aPos;   // Point position
layout (location = 1) in vec4 aColor; // Optional per-point colour

// camera view projection matrix.
uniform mat4 view;
uniform mat4 proj;

out vec4 vColor;

void main()
{
    gl_Position = proj * view * aPos;
    
    // [TODO] T1.2 set vColor to aColor
    vColor = aColor;
}