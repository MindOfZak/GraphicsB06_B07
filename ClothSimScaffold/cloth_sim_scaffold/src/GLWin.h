#ifndef __GL_WIN_H__
#define __GL_WIN_H__

#include <memory>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


class GLWin {
public:
    int width;
    int height;

    static std::unique_ptr<GLWin> createWin(int width = 640, int height = 480, std::string title = "OpenGL Win") {
        std::unique_ptr<GLWin> win = std::make_unique<GLWin>(width, height);
        win->init(title);

        return win;
    }

    GLFWwindow * getGLFWwin() { return window; }

    // should make this constructor private, but need special handling of smart pointers
    GLWin(int w, int h) {
        width = w;
        height = h;
        window = NULL;
    }

private:
    GLFWwindow * window;

    int init(std::string title);

};

#endif