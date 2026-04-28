#ifndef __PARTICLE_SYS_H__
#define __PARTICLE_SYS_H__

#include "glm/gtc/matrix_transform.hpp"

#include "Material.h"
#include "ArcballCamera.h"

struct Particle
{
    glm::vec4 pos;       // 16 bytes
    glm::vec4 velocity;  // 16 bytes
    glm::vec4 color;     // 16 bytes

    float life;          // 4 bytes
    float maxLife;       // 4 bytes

    // padding for alignment of 16 bytes
    int padding1;        // 4 bytes
    int padding2;        // 4 bytes

    //float rotation;
    //float angularVelocity;
};


enum class DrawMode {POINT, TEXTURE, SPRITE};

class ParticleSystem
{
public:

    ParticleSystem();
    ~ParticleSystem();
    
    bool getPlaySim() { return bPlaySim; }
    void setPlaySim(bool bPlay) { bPlaySim = bPlay; }

    void setCamera( std::shared_ptr<ArcballCamera> cam) { camera = cam; }

    void setTexture(std::shared_ptr<Texture> tex) {  
        drawTexMat->setTexture((char*)"tex", tex);
    }

    void setSpriteTexture(std::shared_ptr<Texture> tex, int nx, int ny) {  
        drawSpriteMat->setTexture((char*)"tex", tex);
        drawSpriteMat->setInt("nxTex", nx);
        drawSpriteMat->setInt("nyTex", ny);
    }

    void setDrawPoint() { drawMode = DrawMode::POINT; }
    void setDrawTexture() { drawMode = DrawMode::TEXTURE; }
    void setDrawSprite() { drawMode = DrawMode::SPRITE; }

    // update
    void tick(float dt);

    void draw();

    // Base position of the system.
    glm::vec3 basePos = glm::vec3(0.0);

    // Time in seconds until particles are recycled.
    float maxLife = 1.0f;
    
    // particle acceleration
    glm::vec3 acc = glm::vec3(0.5, 0.5, 0.5);

    // size of particles
    //glm::vec2 partSize = glm::vec2(100, 100);

    float size = 0.2f;

private:
    // The particle system will work with a predefined pool of particles, this makes things way faster than having a dynamic list.
    // You may be able to increase this number depending on your hardware.
    // I was able to run it smoothly with 65536 particles on an NVIDIA Mobile 5070Ti
    // 16348

    bool bPlaySim = true;
    // [TODO] T3.3.3 change NUM_POINTS from 8 to 1024
    static const int NUM_POINTS = 8;
    Particle particles[NUM_POINTS];

    std::shared_ptr<ShaderProgram> progPoint;  // render particles as points
    std::shared_ptr<ShaderProgram> progQuadTex; // render particles as textures
    std::shared_ptr<ShaderProgram> progQuadSprite; // render partcles as texture sprites

    // rendering
    std::shared_ptr<ArcballCamera> camera;

    // particle vertex and fragment shaders attached to materials
    std::shared_ptr<Material> drawPointMat;
    std::shared_ptr<Material> drawTexMat;
    std::shared_ptr<Material> drawSpriteMat;

    // the simulation compute shader
    std::shared_ptr<Material> simMat;

    // quad for drawing particles with GL_TRIANGLE_STRIP
    inline static const GLfloat vertQuad[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
    };

    DrawMode drawMode = DrawMode::POINT;

    // for drawing particles
    GLuint vaoPoint;  // VAO (vertex array object) for point list

    GLuint vaoQuad;  // vertex array object for quad
    GLuint quadVertBuf; // the VBO (vertex buffer object) for the quad

    // buffer for particles
    // use as Vertex Buffers (VBO) or Shader Storage Buffers (SSBO)
    GLuint partBuf;  

    void initBuffers();
    void initBufPoints();
    void initBufQuads();

    void drawPoints();
    void drawQuad();

};

#endif