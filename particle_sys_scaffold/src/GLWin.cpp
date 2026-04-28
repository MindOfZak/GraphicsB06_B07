#include "GLWin.h"
#include "Interaction.h"

extern App app;

void window_size_callback(GLFWwindow* window, int width, int height)
{
    // ignore window minimisation
    if (width <= 1 || height <= 1)
        return;

    glViewport(0, 0, width, height);

    app.camera->SetViewportSize(width, height);
}


int GLWin::init(std::string title) 
{
    // GLFW init
    if (!glfwInit())
    {
        std::cout << "glfw failed" << std::endl;
        return -1;
    }

    // create a GLFW window
    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    glfwMakeContextCurrent(window);

    // register the key event callback function
    glfwSetKeyCallback(window, key_callback);
    
    // register the mouse button event callback function
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, CursorPosCallback);
    glfwSetScrollCallback(window, ScrollCallback);

    glfwSetWindowSizeCallback(window, window_size_callback);

    // loading glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Couldn't load opengl" << std::endl;
        glfwTerminate();
        return -1;
    }

    return 0;

}