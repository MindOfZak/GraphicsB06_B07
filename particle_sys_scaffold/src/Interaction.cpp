#include "Interaction.h"



bool leftDown = false;
bool middleDown = false;
double lastX, lastY;


extern int width;
extern int height;
extern bool bWireframe;

//extern std::vector< std::shared_ptr <Mesh> > meshList;
extern glm::vec3 viewPos_default;
extern glm::mat4 matModelRoot;

extern App app;

extern void key_callback_sim(GLFWwindow* window, int key, int scancode, int action, int mods);

glm::vec3 screenPosToRay(int mouseX, int mouseY, int w, int h,
                         const glm::mat4 &proj, const glm::mat4 &view)
{
    float x = (2.0f * mouseX) / w - 1.0f;
    float y = 1.0f - (2.0f * mouseY) / h;

    // clicked point on the near plane in NDC space
    // if we assume the eye space is scaled so that z_{near} = -1.0
    // then this NDC coordinate is the same as its clip coordinate
    glm::vec4 ray_clip(x, y, -1.0f, 1.0f);

    // set one point with (x, y) and z = -1.0 in eye/camera space
    // the camera is located at (0, 0, 0)
    glm::vec4 ray_eye = glm::inverse(proj) * ray_clip;

    // ray direction in scaled eye space: z_{near} = -1.0
    ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0f, 0.0f);

    // convert the vector to the word space
    glm::vec3 ray_world = glm::normalize(glm::vec3(glm::inverse(view) * ray_eye));
    return ray_world;
}

void mouse_button_callback(GLFWwindow *win, int button, int action, int mods)
{
    //std::cout << "Mouse click at: (" << mx <<", " << my << ")" << std::endl;

    double mx, my;
    glfwGetCursorPos(win, &mx, &my);

    int w, h;
    glfwGetWindowSize(win, &w, &h);

    // for arcball control
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            leftDown = true;
            app.camera->MouseDown(mx / w, my / h);
        } else {
            leftDown = false;
            app.camera->MouseUp();
        }
    }

    lastX = mx;
    lastY = my;

    if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
        if (action == GLFW_PRESS) {
            middleDown = true;
        } else {
            middleDown = false;
        }
    }
    /*
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        glm::vec3 rayOrig = camera->eye;
        glm::vec3 rayDir = screenPosToRay((int)mx, (int)my, w, h, camera->matProj, camera->matView);

        Ray ray{rayOrig, rayDir};

        
        for (std::shared_ptr<Mesh> pMesh : meshList)
        {
            HitInfo hit;
            if (pMesh->pSpatial->Raycast(ray, hit)) {
                std::cout << "Picked triangle index: " << hit.triIndex << ", t=" << hit.t << std::endl;
                pMesh->setPicked(true);
            } else {
                pMesh->setPicked(false);
                //std::cout << "No objects picked" << std::endl;
            }
        }
    }
    */
}

void CursorPosCallback(GLFWwindow* window, double x, double y)
{
    //std::cout << "Cursor Pos Callback" << std::endl;

    double nx = x / width;
    double ny = y / height;

    if (leftDown) {
        app.camera->MouseMove(nx, ny);
    }

    if (middleDown) {
        float dx = float(x - lastX);
        float dy = float(y - lastY);
        app.camera->Pan(dx, dy);
    }

    lastX = x;
    lastY = y;
}


void ScrollCallback(GLFWwindow* window, double xoff, double yoff)
{
    app.camera->Zoom((float)yoff);
}


void key_callback_sim(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //std::cout << "simulation key callback " << std::endl;
    if (action == GLFW_PRESS)
    {
        // Controls
        if (GLFW_KEY_SPACE == key) {
            // there are bugs on disable simulation
            if (app.sim->getPlaySim() )
                app.sim->setPlaySim(false);
            else
                app.sim->setPlaySim(true);
        } else if (GLFW_KEY_1 == key) {
            std::cout << "Draw Particles as Points" << std::endl;
            app.sim->setDrawPoint();
        } else if (GLFW_KEY_2 == key) {
            std::cout << "Draw Particles as Texture" << std::endl;
            app.sim->setDrawTexture();
        } else if (GLFW_KEY_3 == key) {
            std::cout << "Draw Particles as Texture Sprites" << std::endl;
            app.sim->setDrawSprite();
        }
        
    }
}


void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    key_callback_sim(window, key, scancode, action, mods);

    glm::mat4 mat = glm::mat4(1.0);

    float angleStep = 5.0f;
    float transStep = 1.0f;

    if (action == GLFW_PRESS)
    {
        // std::cout << "key pressed" << std::endl;
        /*
        // we don't allow objects to move for picking and collision detection
        if (mods & GLFW_MOD_CONTROL) {
            // translation in world space
            if (GLFW_KEY_LEFT == key) {
                mat = glm::translate(glm::mat4(1.0f), glm::vec3(transStep, 0.0f, 0.0f));
                matModelRoot = mat * matModelRoot;
            } else if (GLFW_KEY_RIGHT == key) {
                mat = glm::translate(glm::mat4(1.0f), glm::vec3(-transStep, 0.0f, 0.0f));
            } else if (GLFW_KEY_UP == key) {
                mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, transStep, 0.0f));
                matModelRoot = mat * matModelRoot;
            } else if (GLFW_KEY_DOWN == key) {
                mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -transStep, 0.0f));
            }
            matModelRoot = mat * matModelRoot;
        }
        */

        if (GLFW_KEY_X == key)
        {
            bWireframe = ! bWireframe;

            if (bWireframe)
                glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            else
                glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        }

        /*
        if (GLFW_KEY_R == key)
        {
            // std::cout << "R pressed" << std::endl;
            //  reset
            app.camera->reset(viewPos_default, glm::vec3(0, 0, 0));
            matModelRoot = glm::mat4(1.0f);

            return;
        }
        */

        // glm::mat4 nextMatView = app.camera->matView;
        // glm::vec3 nextViewPos = app.camera->eye;

        // camera control
        /*
        if (mods & GLFW_MOD_CONTROL) {
            if (GLFW_KEY_LEFT == key) {
                // pan left, rotate around Y, CCW
                mat = glm::rotate(glm::radians(-angleStep), glm::vec3(0.0, 1.0, 0.0));
                nextMatView = mat * camera->matView;
            }
            else if (GLFW_KEY_RIGHT == key) {
                // pan right, rotate around Y, CW
                mat = glm::rotate(glm::radians(angleStep), glm::vec3(0.0, 1.0, 0.0));
                nextMatView = mat * camera->matView;
            }
            else if (GLFW_KEY_UP == key) {
                // tilt up, rotate around X, CCW
                mat = glm::rotate(glm::radians(-angleStep), glm::vec3(1.0, 0.0, 0.0));
                nextMatView = mat * camera->matView;
            }
            else if (GLFW_KEY_DOWN == key) {
                // tilt down, rotate around X, CW
                mat = glm::rotate(glm::radians(angleStep), glm::vec3(1.0, 0.0, 0.0));
                nextMatView = mat * camera->matView;
            }
            else if ((GLFW_KEY_KP_ADD == key) ||
                (GLFW_KEY_EQUAL == key) && (mods & GLFW_MOD_SHIFT)) {
                // zoom in, move along -Z
                mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, transStep));
                nextMatView = mat * camera->matView;
            }
            else if ((GLFW_KEY_KP_SUBTRACT == key) || (GLFW_KEY_MINUS == key)) {
                // zoom out, move along -Z
                mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -transStep));
                nextMatView = mat * camera->matView;
            }
        }

        // translation along camera axis (first person view)
        if (GLFW_KEY_A == key) {
            //  move left along -X
            mat = glm::translate(glm::mat4(1.0f), glm::vec3(transStep, 0.0f, 0.0f));
            nextMatView = mat * camera->matView;
        } else if (GLFW_KEY_D == key) {
            // move right along X
            mat = glm::translate(glm::mat4(1.0f), glm::vec3(-transStep, 0.0f, 0.0f));
            nextMatView = mat * camera->matView;
        } else if (GLFW_KEY_W == key) {
            // move forward along -Z
            mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, transStep));
            nextMatView = mat * camera->matView;
        } else if (GLFW_KEY_S == key) {
            // move backward along Z
            mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -transStep)); 
            nextMatView = mat * camera->matView;
        }
        


        // translation along world axis
        if (GLFW_KEY_LEFT == key) {
            //  move left along -X
            mat = glm::translate(glm::mat4(1.0f), glm::vec3(transStep, 0.0f, 0.0f));
            nextMatView = camera->matView * mat;
            nextViewPos.x -= transStep;
        } else if (GLFW_KEY_RIGHT == key) {
            // move right along X
            mat = glm::translate(glm::mat4(1.0f), glm::vec3(-transStep, 0.0f, 0.0f));
            nextMatView = camera->matView * mat;
            nextViewPos.x += transStep;
        } else if (GLFW_KEY_UP == key) {
            // move up along Y
            mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -transStep, 0.0f));
            nextMatView = camera->matView * mat;
            nextViewPos.y += transStep;
        } else if (GLFW_KEY_DOWN == key) {
            // move down along -Y
            mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, transStep, 0.0f));
            nextMatView = camera->matView * mat;
            nextViewPos.y -= transStep;
        } else  if ((GLFW_KEY_KP_SUBTRACT == key) || (GLFW_KEY_MINUS == key))  {
            // move backward along +Z
            mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -transStep));
            nextMatView = camera->matView * mat;
            nextViewPos.z += transStep;
        } else if ((GLFW_KEY_KP_ADD == key) ||
            (GLFW_KEY_EQUAL == key) && (mods & GLFW_MOD_SHIFT))  {
            // move forward along -Z
            mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, transStep));
            nextMatView = camera->matView * mat;
            nextViewPos.z -= transStep;
        }
        */

        // check collision detection
        /*
        AABB mybox{ nextViewPos - 0.2f, nextViewPos + 0.2f };
        std::vector<int> out;

        bool bCollide = false;
        for (std::shared_ptr<Mesh> pMesh : meshList)
        {
            pMesh->pSpatial->QueryAABB(mybox, out);
            
            if (out.empty()) {
                std::cout << "No collision" << std::endl;
            } else {
                bCollide = true;
                std::cout << "Collision detected: " << out.size() << std::endl;
            }
        }

        if (!bCollide) {
            camera->matView = nextMatView;
            // should use the camera class
            camera->eye = nextViewPos;
        }
        */
    }
}