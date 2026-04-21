#ifndef __RIGID_PLANE__
#define __RIGID_PLANE__

#include "RigidBody.h"
#include "PlaneMesh.h"

// axis aligned planes
class RigidPlane : public RigidBody 
{
public:
    float width = 1.0f;
    float length = 1.0f;

    // plane equation, h can be intepreted as height of the plane
    // a x + b y + c z = h
    glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f);
    float h = 0.0f;

    RigidPlane(float w = 1.0f, float len = 1.0f, float ah = 0.0f, glm::vec3 n = glm::vec3(0.0f, 1.0f, 0.0f) ) {
        width = w;
        length = len;
        h = ah;
        normal = n;

        // setPosition(glm::vec3(0.0f, h, 0.0f));
        setPosition(normal * h);
    }

    // point to plane distance
    float pointDist(glm::vec3 p) {
        return (glm::dot(p, normal) - h) / glm::length(normal);
    }

    //void setNormal(glm::vec3 n) {
    //    normal = n;
    //}

};

#endif