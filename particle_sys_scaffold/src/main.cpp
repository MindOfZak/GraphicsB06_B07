#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp> 

#include "GLWin.h"

#include "shader.h"
#include "Mesh.h"

#include "Interaction.h"
#include "MeshFactory.h"
#include "AppMain.h"
#include "ParticleSys.h"


App app;

//glm::mat4 matModelRoot = glm::mat4(1.0);
glm::vec3 viewPos_default = glm::vec3(0.0f, 0.0f, 100.0f);

// viewport width and height
int width = 800;
int height = 800;

bool bWireframe = false;

int main()
{
    app.glWin = GLWin::createWin(width, height, "Hello Particle Systems");
    
    GLFWwindow *window = app.glWin->getGLFWwin();

    app.camera = std::make_shared<ArcballCamera>(
        viewPos_default,
        glm::vec3(0,0,0), // target
        12.0f,            // distance
        50.0f,            // FOV
        float(width) / float(height),
        0.1f,
        1000.0f
    );

    // ================================================
    // Particle System
    // Initialize the particle system class with a bunch of parameters:
    std::shared_ptr<Texture> tex = std::make_shared<Texture>("models/particle.png");
    std::shared_ptr<Texture> tex_sprite = std::make_shared<Texture>("models/fire_sprite.png");

    std::shared_ptr<ParticleSystem> particleSystem = std::make_shared<ParticleSystem>();

    particleSystem->basePos = glm::vec3(0, -4.0f, 0.0f);
    particleSystem->maxLife = 1.0f;
    particleSystem->size = 0.4f;

    particleSystem->setCamera(app.camera);
    particleSystem->setTexture(tex);
    particleSystem->setSpriteTexture(tex_sprite, 8, 6);

    app.sim = particleSystem;

    // setting the background colour, you can change the value
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glDisable(GL_DEPTH_TEST);
    //glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );


    std::cout << "==================================" << std::endl;
    std::cout << "SPACE: Pause/continue simulation" << std::endl;
    std::cout << "1: Draw Particles as Points " << std::endl;
    std::cout << "2: Draw Particles Using One Single Texure " << std::endl;
    std::cout << "3: Draw Particles Using Texure Sprites " << std::endl;
    std::cout << "==================================" << std::endl;


    // setting the event loop
    while (!glfwWindowShouldClose(window))
    {
        if (nullptr == app.sim)  continue;

        glClear(GL_COLOR_BUFFER_BIT);

        // update simulation using 
        // a fixed time step
        app.sim->tick(0.005);

        // draw the models
        app.sim->draw();

        glfwSwapBuffers(window);

        glfwPollEvents();

    }

    glfwTerminate();

    return 0;
}


