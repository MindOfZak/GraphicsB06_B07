#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp> 

#include "shader.h"
#include "Mesh.h"
#include "Node.h"
#include "Animation.h"
#include "Animator.h"




static Shader shader;

// Attempt 1 to add 2 characters to the scene with animations with on model matrix 
glm::mat4 swatModelMatrix = glm::mat4(1.0f);
glm::mat4 vampireModelMatrix = glm::mat4(1.0f);

glm::mat4 matModelRoot = glm::mat4(1.0);
glm::mat4 matView = glm::mat4(1.0);
glm::mat4 matProj = glm::ortho(-2.0f,2.0f,-2.0f,2.0f, -2.0f,2.0f);

glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0);

// camera settings
glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 200.0f);
glm::vec3 viewPos = glm::vec3(0.0f, 120.0f, 300.0f);
glm::vec3 viewCenter = glm::vec3(30.0f, 80.0f, 0.0f);
int wView = 800;
int hView = 800;
float fov = 70.0;
float near = 0.1;
float far = 400.0;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool animate = true;

GLuint blinnShader;
GLuint texblinnShader;
GLuint normalblinnShader;
// GLuint boneShader;
GLuint boneShader;
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

void window_size_callback(GLFWwindow* window, int w, int h)
{
    //int width, height;
    //glfwGetWindowSize(window, &width, &height);

    wView = w;
    hView = h;

    glViewport(0, 0, w, h);

    matProj = glm::perspective(glm::radians(fov), w /(float) h, near, far);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    glm::mat4 mat = glm::mat4(1.0);

    float angleStep = 5.0f;
    float transStep = 1.0f;

    if (action == GLFW_PRESS) {

        if (mods & GLFW_MOD_CONTROL) {

            // translation in world space
            if (GLFW_KEY_LEFT == key) {
                mat = glm::translate(glm::mat4(1.0f), glm::vec3(transStep, 0.0f, 0.0f));
                matModelRoot = mat * matModelRoot;
            }
            else if (GLFW_KEY_RIGHT == key) {
                mat = glm::translate(glm::mat4(1.0f), glm::vec3(-transStep, 0.0f, 0.0f));
                matModelRoot = mat * matModelRoot;
            }
            else if (GLFW_KEY_UP == key) {
                mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, transStep, 0.0f));
                matModelRoot = mat * matModelRoot;
            } if (GLFW_KEY_DOWN == key) {
                mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -transStep, 0.0f));
                matModelRoot = mat * matModelRoot;
            };
        }
// ZM1.1.3: Add something like this; ( This assigns Keybinds to swap animations)
       /* if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
          animator.PlayAnimation(&danceAnimation);' 
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
            animator.PlayAnimation(&walkAnimation);

        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
             animator.PlayAnimation(&idleAnimation);
        }*/

        // camera control
        if (GLFW_KEY_LEFT == key) {
            // pan left, rotate around Y, CCW
            mat = glm::rotate(glm::radians(-angleStep), glm::vec3(0.0, 1.0, 0.0));
            matView = mat * matView;
        } else if (GLFW_KEY_RIGHT == key ) {
            // pan right, rotate around Y, CW
            mat = glm::rotate(glm::radians(angleStep), glm::vec3(0.0, 1.0, 0.0));
            matView = mat * matView;
        } else if (GLFW_KEY_UP == key) {
            // tilt up, rotate around X, CCW
            mat = glm::rotate(glm::radians(-angleStep), glm::vec3(1.0, 0.0, 0.0));
            matView = mat * matView;
        } if (GLFW_KEY_DOWN == key) {
            // tilt down, rotate around X, CW
            mat = glm::rotate(glm::radians(angleStep), glm::vec3(1.0, 0.0, 0.0));
            matView = mat * matView;
        } else if ( (GLFW_KEY_KP_ADD == key) || 
            (GLFW_KEY_EQUAL == key) && (mods & GLFW_MOD_SHIFT) ) {
            // std::cout << "+ pressed" << std::endl;
            // zoom in, move along -Z
            mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, transStep));
            matView = mat * matView ;
        } else if ( (GLFW_KEY_KP_SUBTRACT == key ) || (GLFW_KEY_MINUS == key) ) {
            // std::cout << "keypad - pressed" << std::endl;
            // zoom out, move along -Z
            mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -transStep));
            matView = mat * matView;
        } else if (GLFW_KEY_R == key) {
            //std::cout << "R pressed" << std::endl;
            // reset
            matView = glm::lookAt(viewPos, viewCenter, glm::vec3(0, 1, 0)); 
            matModelRoot = glm::mat4(1.0f);
        }

        // translation along camera axis (first person view)
        else if (GLFW_KEY_A == key ) {
            //if (modes & GLFW_MOD_CONTROL)
            // move left along -X
            mat = glm::translate(glm::mat4(1.0f), glm::vec3(transStep, 0.0f, 0.0f));
            matView = mat * matView;
        } else if (GLFW_KEY_D == key) {
            // move right along X
            mat = glm::translate(glm::mat4(1.0f), glm::vec3(-transStep, 0.0f, 0.0f));
            matView = mat * matView;
        } if (GLFW_KEY_W == key ) {
            // move forward along -Z
            mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, transStep));
            matView = mat * matView;
        } else if (GLFW_KEY_S == key) {
            // move backward along Z
            mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -transStep));
            matView = mat * matView;
        } 

        // translation along world axis
        else if (GLFW_KEY_H == key ) {
            //if (modes & GLFW_MOD_CONTROL)
            // move left along -X
            mat = glm::translate(glm::mat4(1.0f), glm::vec3(transStep, 0.0f, 0.0f));
            matView = matView * mat;
        } else if (GLFW_KEY_L == key) {
            // move right along X
            mat = glm::translate(glm::mat4(1.0f), glm::vec3(-transStep, 0.0f, 0.0f));
            matView = matView * mat;
        } if (GLFW_KEY_J == key ) {
            // move forward along Z
            mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -transStep));
            matView = matView * mat;
        } else if (GLFW_KEY_K == key) {
            // move backward along -Z
            mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, transStep));
            matView = matView * mat;
        } 
    }
    
}

int main()
{
    GLFWwindow *window;

    // GLFW init
    if (!glfwInit())
    {
        std::cout << "glfw failed" << std::endl;
        return -1;
    }

    // create a GLFW window
    window = glfwCreateWindow(wView, hView, "Hello Skeletal Animation", NULL, NULL);
    glfwMakeContextCurrent(window);

    // register the key event callback function
    glfwSetKeyCallback(window, key_callback);

    glfwSetWindowSizeCallback(window, window_size_callback);


    // loading glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Couldn't load opengl" << std::endl;
        glfwTerminate();
        return -1;
    }

    


    blinnShader = initShader( "shaders/blinn.vert", "shaders/blinn.frag");
    setLightPosition(lightPos);
    setViewPosition(viewPos);

    texblinnShader = initShader("shaders/texblinn.vert", "shaders/texblinn.frag");
    setLightPosition(lightPos);
    setViewPosition(viewPos);

    //normalblinnShader = initShader("shaders/normalblinn2.vert", "shaders/normalblinn2.frag");
    //setLightPosition(lightPos);
    //setViewPosition(viewPos);

    // boneShader
    boneShader = initShader("shaders/bone.vert", "shaders/bone.frag");
    setLightPosition(lightPos);
    setViewPosition(viewPos);

    // set the eye at (0, 0, 5), looking at the centre of the world
    matView = glm::lookAt(viewPos, viewCenter, glm::vec3(0, 1, 0)); 

    // set the Y field of view angle to 60 degrees, width/height ratio to 1.0, and a near plane of 3.5, far plane of 6.5
    matProj = glm::perspective(glm::radians(fov), wView / (float) hView, near, far);

    // ZM1.1: to add another animation just simply copy Animation line and then change it to new animation file path. then follow ZM1.1.2 Below. 
	// ZM1.2: Make 2 models by copying below but changing the anim_model to different character name E.G Paladin_model with whatever animation.
    
	// swat character animation
    std::shared_ptr<Mesh> swat_model = std::make_shared<Mesh>();
    swat_model->init("models/Surprise_Uppercut_Swat_Hit/SurpriseUppercutSwat.dae", boneShader);
    Animation SwatUppercutAnimation("models/Surprise_Uppercut_Swat_Hit/SurpriseUppercutSwat.dae", swat_model.get());

	
    // vampire character animation
    std::shared_ptr<Mesh> vampire_model = std::make_shared<Mesh>();
    vampire_model->init("models/Surprise_Uppercut_Vampire_Knocked/SurpriseUppercutVampire.dae", boneShader);
    Animation VampireUppercutAnimation("models/Surprise_Uppercut_Vampire_Knocked/SurpriseUppercutVampire.dae", vampire_model.get());
    
   

	// ZM1.1.2: Then copy the code below and add the new animation here. Then Follow ZM1.1.3 below for keybinds for switching animation.
    Animator swatAnimator(&SwatUppercutAnimation);
    Animator vampireAnimator(&VampireUppercutAnimation);

    // swatModelMatrix
    swatModelMatrix = glm::translate(swatModelMatrix, glm::vec3(-180.0f, 0.0f, 0.0f));
    swatModelMatrix = glm::rotate(swatModelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    swatModelMatrix = glm::scale(swatModelMatrix, glm::vec3(100.0f, 100.0f, 100.0f));
    // vampireModelMatrix
    vampireModelMatrix = glm::translate(vampireModelMatrix, glm::vec3(-120.0f, 0.0f, 0.0f));
    vampireModelMatrix = glm::rotate(vampireModelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    vampireModelMatrix = glm::scale(vampireModelMatrix, glm::vec3(100.0f, 100.0f, 100.0f));





    // setting the background colour, you can change the value
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    
    glEnable(GL_DEPTH_TEST);

    if (animate) {
        scale = glm::vec3(100.0, 100.0, 100.0);
        matModelRoot = glm::scale(matModelRoot, scale);
    }

    // setting the event loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

// ZM1.3: add new character animator, copy and paste but change names.
        if (animate)
        {
            swatAnimator.UpdateAnimation(deltaTime);
            vampireAnimator.UpdateAnimation(deltaTime);
        }

        glUseProgram(boneShader);

// ZM1.4: Copy and past all below and change the name of the character animator to new one also, 'glfwSwapBuffers(window);' stays underneath all of it so dont copy.
        // update bone matrices in the shader
        auto swatTransforms = swatAnimator.GetFinalBoneMatrices();
        for (int i = 0; i < swatTransforms.size(); ++i) {
            glm::mat4 mat = swatTransforms[i];
            std::string name = "finalBonesMatrices[" + std::to_string(i) + "]";
            glUniformMatrix4fv(glGetUniformLocation(boneShader, name.c_str()), 1, GL_FALSE, &swatTransforms[i][0][0]);

        }
        swat_model->draw(swatModelMatrix, matView, matProj);

        auto vampireTransforms = vampireAnimator.GetFinalBoneMatrices();
        for (int i = 0; i < vampireTransforms.size(); ++i) {
            glm::mat4 mat = vampireTransforms[i];
            std::string name = "finalBonesMatrices[" + std::to_string(i) + "]";
            glUniformMatrix4fv(glGetUniformLocation(boneShader, name.c_str()), 1, GL_FALSE, &vampireTransforms[i][0][0]);

        }
        vampire_model->draw(vampireModelMatrix, matView, matProj);
        glfwSwapBuffers(window);

        //break;
    }

    glfwTerminate();

    return 0;
}