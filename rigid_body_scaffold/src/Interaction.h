#ifndef __INTERACTION_H__
#define __INTERACTION_H__

#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


#include "ArcballCamera.h"
#include "Mesh.h"
#include "AppMain.h"


glm::vec3 screenPosToRay(int mouseX, int mouseY, int w, int h,
                         const glm::mat4 &proj, const glm::mat4 &view);

void mouse_button_callback(GLFWwindow *win, int button, int action, int mods);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void key_callback_sim(GLFWwindow *window, int key, int scancode, int action, int mods);
void ScrollCallback(GLFWwindow* window, double xoff, double yoff);
void CursorPosCallback(GLFWwindow* window, double x, double y);



#endif