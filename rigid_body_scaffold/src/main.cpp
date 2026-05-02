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
//#include "ClothSim.h"
#include "AppMain.h"
#include "SphereMesh.h"
#include "RigidSphere.h"
#include "PlaneMesh.h"
#include "RigidPlane.h"


App app;

static Shader shader;

glm::mat4 matModelRoot = glm::mat4(1.0);

glm::vec3 lightPos = glm::vec3(5.0f, 5.0f, 10.0f);
glm::vec3 viewPos_default = glm::vec3(0.0f, 0.0f, 100.0f);

GLuint blinnShader;
GLuint texblinnShader;

// viewport width and height
int width = 800;
int height = 800;

bool bWireframe = false;

std::shared_ptr<RigidBody> cueBallObj = nullptr;



// Initialize shader
GLuint initShader(std::string pathVert, std::string pathFrag) 
{
    shader.read_source( pathVert.c_str(), pathFrag.c_str());

    shader.compile();
    glUseProgram(shader.program);

    return shader.program;
}

void setLightPosition(glm::vec3 lightPos)
{
    GLuint lightpos_loc = glGetUniformLocation(shader.program, "lightPos" );
    glUniform3fv(lightpos_loc, 1, glm::value_ptr(lightPos));
}

void setViewPosition(glm::vec3 eyePos)
{
    GLuint viewpos_loc = glGetUniformLocation(shader.program, "viewPos" );
    glUniform3fv(viewpos_loc, 1, glm::value_ptr(eyePos));
}

void init_singleBall(glm::vec3 v = glm::vec3(0.0), bool bGround = false)
{
    app.sim->clear();

    float radius = 4.0f;
    std::shared_ptr<SphereMesh> ball = std::make_shared<SphereMesh>(20, 20, radius);
    ball->setShaderId(blinnShader);

    std::shared_ptr<RigidSphere> ball1 = std::make_shared<RigidSphere>(radius);
    ball1->setMesh(ball);
    std::shared_ptr<RigidBody> ball1Obj = std::dynamic_pointer_cast<RigidBody>(ball1);
    ball1Obj->setPosition(glm::vec3(0.0f, 50.0f, 0.0f));
    ball1Obj->setVelocity(v);
    app.sim->add(ball1Obj);

    if (bGround) {
        float width = 100.0f;
        float length = 100.0f;

        std::shared_ptr<PlaneMesh> planeMesh = std::make_shared<PlaneMesh>(PlaneMesh::XZ, width, length, 10, 10, -width / 2.0, -length / 2.0);
        planeMesh->setShaderId(blinnShader);
        planeMesh->initBuffer();

        std::shared_ptr<RigidPlane> planeRigid = std::make_shared<RigidPlane>(width, length, -20.0f);
        planeRigid->setDynamic(false);
        planeRigid->setMesh(planeMesh);
        std::shared_ptr<RigidBody> planeObj = std::dynamic_pointer_cast<RigidBody>(planeRigid);
        app.sim->add(planeObj);
    }
}


void init_twoBall(glm::vec3 v = glm::vec3(0.0f))
{
    app.sim->clear();

    float radius = 4.0f;
    std::shared_ptr<SphereMesh> ball = std::make_shared<SphereMesh>(20, 20, radius);
    ball->setShaderId(blinnShader);

    std::shared_ptr<RigidSphere> ball1 = std::make_shared<RigidSphere>(radius);
    ball1->setMesh(ball);
    std::shared_ptr<RigidBody> ball1Obj = std::dynamic_pointer_cast<RigidBody>(ball1);
    ball1Obj->setPosition(glm::vec3(0.0f, 50.0f, 0.0f));
    ball1Obj->setVelocity(v);
    app.sim->add(ball1Obj);


    float width = 100.0f;
    float length = 100.0f;

    std::shared_ptr<PlaneMesh> planeMesh = std::make_shared<PlaneMesh>(PlaneMesh::XZ, width, length, 10, 10, -width / 2.0, -length / 2.0);
    planeMesh->setShaderId(blinnShader);
    planeMesh->initBuffer();


    std::shared_ptr<RigidSphere> ball2 = std::make_shared<RigidSphere>(radius);
    ball2->setMesh(ball);
    std::shared_ptr<RigidBody> ball2Obj = std::dynamic_pointer_cast<RigidBody>(ball2);
    ball2Obj->setPosition(glm::vec3(-3.0f, 0.0f, 0.0f));
    //ball2Obj->setVelocity(glm::vec3(-0.5, 0.0f, 0.0f));
    ball2Obj->setUseGravity(false);
    app.sim->add(ball2Obj);


    std::shared_ptr<RigidPlane> planeRigid = std::make_shared<RigidPlane>(width, length, -20.0f);
    planeRigid->setDynamic(false);
    planeRigid->setMesh(planeMesh);
    std::shared_ptr<RigidBody> planeObj = std::dynamic_pointer_cast<RigidBody>(planeRigid);
    app.sim->add(planeObj);
}

// My Attempt at making a snooker table, not sure if it works yet
void init_snooker() {
    app.sim->clear();

    float radius = 4.0f;

    auto makeBall = [&](glm::vec3 colour) {
        std::shared_ptr<SphereMesh> mesh =
            std::make_shared<SphereMesh>(20, 20, radius);

        mesh->setShaderId(blinnShader);
        mesh->setColour(colour);
        mesh->initBuffer();

        std::shared_ptr<RigidSphere> sphere =
            std::make_shared<RigidSphere>(radius);

        sphere->setMesh(mesh);

        std::shared_ptr<RigidBody> body =
            std::dynamic_pointer_cast<RigidBody>(sphere);

        body->setMass(1.0f);
        body->setUseGravity(false);

        return body;
        };

    float width = 300.0f;
    float length = 180.0f;

    std::shared_ptr<PlaneMesh> planeMesh =
        std::make_shared<PlaneMesh>(
            PlaneMesh::XZ,
            width,
            length,
            10,
            10,
            -width / 2.0f,
            -length / 2.0f
        );

    planeMesh->setShaderId(blinnShader);
    planeMesh->setColour(glm::vec3(0.0f, 0.45f, 0.0f));
    planeMesh->initBuffer();

    std::shared_ptr<RigidPlane> table =
        std::make_shared<RigidPlane>(width, length, -4.0f);

    table->setDynamic(false);
    table->setMesh(planeMesh);

    std::shared_ptr<RigidBody> tableObj =
        std::dynamic_pointer_cast<RigidBody>(table);

    app.sim->add(tableObj);

    // cue ball
    cueBallObj = makeBall(glm::vec3(1.0f, 1.0f, 1.0f));

    cueBallObj->setPosition(glm::vec3(-120.0f, 0.0f, 0.0f));
    cueBallObj->setVelocity(glm::vec3(35.0f, 0.0f, 0.0f));

    app.sim->add(cueBallObj);

    // pool ball colours
    std::vector<glm::vec3> poolColours = {
        glm::vec3(1.0f, 1.0f, 0.0f),   // yellow
        glm::vec3(0.0f, 0.1f, 1.0f),   // blue
        glm::vec3(1.0f, 0.0f, 0.0f),   // red
        glm::vec3(0.5f, 0.0f, 0.8f),   // purple
        glm::vec3(1.0f, 0.45f, 0.0f),  // orange
        glm::vec3(0.0f, 0.6f, 0.0f),   // green
        glm::vec3(0.5f, 0.0f, 0.0f),   // maroon
        glm::vec3(0.0f, 0.0f, 0.0f),   // black
        glm::vec3(1.0f, 1.0f, 0.0f),   // yellow
        glm::vec3(0.0f, 0.1f, 1.0f),   // blue
        glm::vec3(1.0f, 0.0f, 0.0f),   // red
        glm::vec3(0.5f, 0.0f, 0.8f),   // purple
        glm::vec3(1.0f, 0.45f, 0.0f),  // orange
        glm::vec3(0.0f, 0.6f, 0.0f),   // green
        glm::vec3(0.5f, 0.0f, 0.0f)    // maroon
    };

    // triangle rack
    glm::vec3 rackStart = glm::vec3(60.0f, 0.0f, 0.0f);

    int colourIndex = 0;

    for (int row = 0; row < 5; row++) {
        for (int col = 0; col <= row; col++) {

            auto rackObj = makeBall(poolColours[colourIndex]);

            float x = rackStart.x + row * radius * 2.0f;
            float y = 0.0f;
            float z = rackStart.z + (col - row * 0.5f) * radius * 2.1f;

            rackObj->setPosition(glm::vec3(x, y, z));
            rackObj->setVelocity(glm::vec3(0.0f));

            app.sim->add(rackObj);

            colourIndex++;
        }
    }
}

void clearScene() 
{
    app.sim->clear();
}

void key_callback_sim(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //std::cout << "simulation key callback " << std::endl;
    if (action == GLFW_PRESS)
    {
        // Controls
        if (GLFW_KEY_SPACE == key) {
           app.sim->setPlaySim(true);

           if (cueBallObj != nullptr) {
               cueBallObj->applyLinearImpulse(glm::vec3(35.0f, 0.0f, 0.0f));
		   }
        } else if (GLFW_KEY_1 == key) {
            init_singleBall();
        } else if (GLFW_KEY_2 == key) {
            init_singleBall(glm::vec3(-8.0f, 0.0f, 0.0f));
        } else if (GLFW_KEY_3 == key) {
            init_singleBall(glm::vec3(0.0f), true);
        } else if (GLFW_KEY_4 == key) {
            init_singleBall(glm::vec3(-4.0f, 0.0f, 0.0f), true);
        } else if (GLFW_KEY_5 == key) {
            init_twoBall(glm::vec3(0.0f));
        } else if (GLFW_KEY_6 == key) {
            init_snooker();

            app.camera->reset(
                glm::vec3(0.0f, 350.0f, 0.1f),  // top down view
                glm::vec3(0.0f, 0.0f, 0.0f),  // almost centre of table view
                glm::vec3(0.0f, 0.0f, -1.0f)   // up vector along z axis
            );

            setViewPosition(app.camera->eye);
        } if (GLFW_KEY_R == key) {
           app.sim->clear();
           app.camera->reset(viewPos_default,
                  glm::vec3(0,0,0)  // target
              );
        }

        //if (GLFW_KEY_G == key)
        //    app.sim->setInverseGravity();
        
    }
}

int main()
{
    app.glWin = GLWin::createWin(width, height, "Hello Rigid Body");
    
    GLFWwindow *window = app.glWin->getGLFWwin();

    app.camera = std::make_shared<ArcballCamera>(
        glm::vec3(0.0f,300.0f,0.1f),  // top down view
		glm::vec3(0, 0, 0),  // almost centre of table view
        300.0f,            // camera distance
        45.0f,             // FOV
        float(width) / float(height),
        0.1f,
        2000.0f
    );

    blinnShader = initShader("shaders/blinn.vert", "shaders/blinn.frag");
    setLightPosition(lightPos);
    setViewPosition(app.camera->eye);

    app.sim = std::make_shared<RigidSim>();

    // init single ball with ground
    init_singleBall(glm::vec3(0.0f), true);


    // setting the background colour, you can change the value
    glClearColor(0.25f, 0.5f, 0.75f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );


    std::cout << "==================================" << std::endl;
    std::cout << "SPACE: Start simulation" << std::endl;
    std::cout << "1: Single ball free falling " << std::endl;
    std::cout << "2: Single ball falling with initial horizontal velocity" << std::endl;
    std::cout << "3: Single ball free falling with ground" << std::endl;
    std::cout << "4: Single ball falling with initial horizontal velocity and ground" << std::endl;
    std::cout << "5: Two balls with ground, one falling" << std::endl;
    std::cout << "6: Snooker balls (no gravity)  hit by one ball falling" << std::endl;
    std::cout << "x: Toggle wireframe" << std::endl;
    std::cout << "r: Clear" << std::endl;
    std::cout << "==================================" << std::endl;

    // setting the event loop
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // update physics
        // time step of 0.005 second
        app.sim->tick(0.005);

        // draw the models
        app.sim->draw();

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}


