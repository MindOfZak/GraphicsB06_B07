#include "ParticleSys.h"

#include <GLFW/glfw3.h>
#include <memory>
#include "ShaderProgram.h"

ParticleSystem::ParticleSystem()
{
    // set up the compute shader material for the particlue simulation
    std::shared_ptr<ShaderProgram> simProgram = std::make_shared<ShaderProgram>();
    std::shared_ptr<ShaderSingle> simShader 
            = std::make_shared<ShaderSingle>("shaders/particle/fire.comp", GL_COMPUTE_SHADER);
    simProgram->attachShader(simShader);
    simMat = std::make_shared<Material>(simProgram);

    // Setup shaders and shader program.
    progPoint = std::make_shared<ShaderProgram>();
    std::shared_ptr<ShaderSingle> vertexShader 
            = std::make_shared<ShaderSingle>("shaders/particle/point.vert", GL_VERTEX_SHADER);
    progPoint->attachShader(vertexShader);
    std::shared_ptr<ShaderSingle> fragShader 
        = std::make_shared<ShaderSingle>("shaders/particle/point.frag", GL_FRAGMENT_SHADER);
    progPoint->attachShader(fragShader);

    drawPointMat = std::make_shared<Material>();
    drawPointMat->setShaderProgram(progPoint);


    progQuadTex = std::make_shared<ShaderProgram>();
    vertexShader = std::make_shared<ShaderSingle>("shaders/particle/quad.vert", 
                                                    GL_VERTEX_SHADER);
    progQuadTex->attachShader(vertexShader);
    fragShader = std::make_shared<ShaderSingle>("shaders/particle/quad.frag", 
                                                    GL_FRAGMENT_SHADER);
    progQuadTex->attachShader(fragShader);

    drawTexMat = std::make_shared<Material>();
    drawTexMat->setShaderProgram(progQuadTex);

    progQuadSprite = std::make_shared<ShaderProgram>();
    vertexShader = std::make_shared<ShaderSingle>("shaders/particle/quad_sprite.vert", 
                                                    GL_VERTEX_SHADER);
    progQuadSprite->attachShader(vertexShader);
    fragShader = std::make_shared<ShaderSingle>("shaders/particle/quad_sprite.frag", 
                                                    GL_FRAGMENT_SHADER);
    progQuadSprite->attachShader(fragShader);

    drawSpriteMat = std::make_shared<Material>();
    drawSpriteMat->setShaderProgram(progQuadSprite);
    
    // Create particle data.
    // [TODO] set particle positions
    for (int i = 0; i < NUM_POINTS; i++)
    {
        // Get a reference to that particle, not a copy.
        Particle & p = particles[i];

        // random position for point rendering
        float x = ((rand() % 100) / 10.0f) - 5.0f;
        float y = ((rand() % 100) / 10.0f) - 5.0f;

        // set the initial particle position 
        // to a random position (x, y, 0.0, w = 1.0)
        // this random position is only for debugging
        // it will be overrided in the compute shader later
        p.pos = glm::vec4(x, y, 0, 1.0);

        p.velocity = glm::vec4(0, 0, 0, 0);

        // assigning random colours for points
        // the colors will be overrided in the compute shader later
        p.color = glm::vec4((rand() % 256) / 255.0f, 
                (rand() % 256) / 255.0f, (rand() % 256) / 255.0f, 1);
        
        p.life = maxLife;
        p.maxLife = maxLife;
    }

    initBuffers();
}

ParticleSystem::~ParticleSystem()
{
    glDeleteVertexArrays(1, &vaoPoint);
    glDeleteVertexArrays(1, &vaoQuad);
    glDeleteBuffers(1, & quadVertBuf);
    glDeleteBuffers(1, & partBuf);
}

void ParticleSystem::initBufPoints()
{
    // vertex array object for the quad and all particle instances
    glGenVertexArrays(1, & vaoPoint);
    glBindVertexArray(vaoPoint);

    // use the particle buffer to specify vertex array attributes
    glBindBuffer(GL_ARRAY_BUFFER, partBuf);
    // glBufferData(GL_ARRAY_BUFFER, NUM_POINTS * sizeof(Particle), particles, GL_DYNAMIC_DRAW);

    // set the vertex attribute pointer for particle positions as attribute 0
    // positions of particles' centers
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)0);

    // set the vertex attribute pointer for particle colours as attribute 1
    glEnableVertexAttribArray(1);
    // [TODO] T1.1 
    // set the colour attribute using
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *) offsetof(Particle, color)); 
    // Hint: the last argument, i.e., offset of a field within Particle
    // can be obtained using (void*)offsetof(Particle, field_name)


    glBindVertexArray(1);
}


void ParticleSystem::initBufQuads()
{
    // vertex array object for the quad and all particle instances
    glGenVertexArrays(1, & vaoQuad);
    glBindVertexArray(vaoQuad);

    // Quad VBO for instancing
    glGenBuffers(1, &quadVertBuf);
    glBindBuffer(GL_ARRAY_BUFFER, quadVertBuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertQuad), vertQuad, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindVertexArray(0);

}

void ParticleSystem::initBuffers()
{
    // create the particle buffer
    glGenBuffers(1, & partBuf);
    glBindBuffer(GL_ARRAY_BUFFER, partBuf);
    glBufferData(GL_ARRAY_BUFFER, NUM_POINTS * sizeof(Particle), particles, GL_DYNAMIC_DRAW);

    initBufPoints();
    initBufQuads();
}


// use the compute shader
void ParticleSystem::tick(float dt)
{
    if (! bPlaySim ) return;

    // Same as with drawing, but we bind a compute shader program instead.
    // Set a bunch of values in the compute shader to use.
    simMat->setFloat((char*)"dt", dt);
    float time = (float) glfwGetTime();
    simMat->setFloat((char*)"time", time);
    simMat->setVec3((char*)"basePos", basePos);
    simMat->setVec3((char*)"acc", acc);
    
	// use the compute shader
	simMat->bind();

    // [TODO] 2.3 Complete 
    // Binding the particle buffer from our square.
    // [TODO] 2.3.1 use glBindBufferBase to bind partBuf to location 0
     glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, partBuf);


    // [TODO] 2.3.2 run the compute shader
    // by calling void glDispatchCompute(num_grp_x,num_grp_y, num_grp_z)
    // here we set num_grp_x to the number of particles,
    // and num_grp_y and num_grp_z to 1
    glDispatchCompute(NUM_POINTS, 1, 1);
    // ask rendering wait for completion of the compute shader
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    simMat->unbind();
}

void ParticleSystem::drawPoints()
{
    glBindVertexArray(vaoPoint);

    glPointSize(50.0f * size);

    // call glDrawArrays to draw points
    glDrawArrays(GL_POINTS, 0, NUM_POINTS);

    glBindVertexArray(0);
}

void ParticleSystem::drawQuad()
{
    glBindVertexArray(vaoQuad);

    // [TODO] 3.1
    // call glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ?? index, ?? buf);
    // bind buffer partBuf to location index 0
     glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0 , partBuf);

    // [TODO] 3.2 
    // draw instanced quads as triangle strips  for all particles
    // call glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, ?? your_count, NUM_POINTS);
    // the missing count should be the number of quad vertices needed in the quad vertex buffer
    // in this example, we are drawing the quad as a triangle strip, only 4 vertices are needed.
    // if you are representing the quad as two triangles with 6 vertices, you may need to use 6.
     glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, NUM_POINTS);


    glBindVertexArray(0);

}



void ParticleSystem::draw()
{
    std::shared_ptr<Material> currentMat;

    switch (drawMode)
    {
        case DrawMode::POINT:
            currentMat = drawPointMat;
            break;
        case DrawMode::TEXTURE:
            currentMat = drawTexMat;
            break;
        case DrawMode::SPRITE: 
            currentMat = drawSpriteMat;
            break;  
    }

    currentMat->setMat4("view", camera->matView);
    currentMat->setMat4("proj", camera->matProj);
    currentMat->setFloat("size", size);

    currentMat->bind();
 
    // [TODO] 3.3 Enable Blending
    // T3.3.1 Enable blending by calling glEnable(GL_BLEND);
    glEnable(GL_BLEND);

    // T3.3.2 specifiy the blend function using glBendFunc(src_factor, dst_factor)
    // the src_factor is normally GL_SRC_ALPHA
    // for rendeing fires with a black background, a dst_factor of GL_ONE is recommended
    // if you are going render smoke with a white background
    // you might need to use a dst_factor of GL_ONE_MINUS_SRC_ALPHA
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);



    switch (drawMode)
    {
        case DrawMode::POINT:
            drawPoints();
            break;
        case DrawMode::TEXTURE:
        case DrawMode::SPRITE: 
            drawQuad();
            break;
    }

    // unuse the shader programs
    currentMat->unbind();

    glDisable(GL_BLEND);
}