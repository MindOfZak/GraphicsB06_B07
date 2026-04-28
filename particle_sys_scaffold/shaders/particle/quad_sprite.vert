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

// for texture sprites
uniform int nxTex;
uniform int nyTex;

uniform float size;

out vec4 pColor;
out vec2 uv;


void main() {

    // Get particle using instance ID
    Particle p = particles[gl_InstanceID];

    vec3 worldPos = p.pos.xyz + aPos * size;
    gl_Position = proj * view * vec4(worldPos, 1.0);

    pColor = p.color;

    if (gl_VertexID % 4 == 0) uv = vec2(0,0);
    if (gl_VertexID % 4 == 1) uv = vec2(1,0);
    if (gl_VertexID % 4 == 2) uv = vec2(0,1);
    if (gl_VertexID % 4 == 3) uv = vec2(1,1);

    
    // [TODO] 4 using texure sprites
    // T4.1 calcuate the scaling based on nxTex, nyTex
    // replace the following line with yours
    vec2 uvScale = vec2(1.0, 1.0);

    // calcuate the sprite index
    // the upper left corner represents the beginning of a particle
    // at (p.life == p.maxLife)
    uint n = int( (1 - p.life / p.maxLife) * nxTex * nyTex);
    
    // T4.2 calcuate offset_x and offset_y of the sprite based on n
    // replace the next two lines with yours.
    uint offset_x = 0;
    uint offset_y = 0;

    // the sprite offset in the texture
    vec2 uvOffset = vec2(offset_x, offset_y) * uvScale;

    // the final texture uv coordinates corresponding to that sprite
    uv = uv * uvScale + uvOffset;

}
