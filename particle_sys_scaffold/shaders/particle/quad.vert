#version 430 core

layout(location = 0) in vec3 aPos;      // quad vertex

struct Particle {
    vec4 pos;
    vec4 velocity;
    vec4 color;
    float life;
    float maxLife;
    int padding1;
    int padding2;
};

layout(std430, binding = 0) buffer Particles {
    Particle particles[];
};

uniform mat4 view;
uniform mat4 proj;

uniform float size;

out vec4 pColor;
out vec2 uv;

void main() {

    // Get particle using instance ID
    Particle p = particles[gl_InstanceID];

    vec3 worldPos = p.pos.xyz + aPos * size;
    gl_Position = proj * view * vec4(worldPos, 1.0);

    // use the particle colour as quad vertex colour
    pColor = p.color;

    // specify the texture coordinates for the four corners of the quad
    if (gl_VertexID % 4 == 0) uv = vec2(0,0);
    if (gl_VertexID % 4 == 1) uv = vec2(1,0);
    if (gl_VertexID % 4 == 2) uv = vec2(0,1);
    if (gl_VertexID % 4 == 3) uv = vec2(1,1);

}
