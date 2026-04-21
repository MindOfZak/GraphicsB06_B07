#ifndef __RIGID_SPHERE_H__
#define __RIGID_SPHERE_H__

#include <memory>
#include "RigidBody.h"
#include "SphereMesh.h"
#include "RigidPlane.h"

class RigidSphere : public RigidBody {

public:
    float r = 1.0f; // radius
    
    RigidSphere() { init(1.0f); }
    RigidSphere(float ar) { init(ar); }

    // return the inertia matrix for angular effects
    glm::mat3 calcInertia() override;

    CollisionInfo testCollisionWith(std::shared_ptr<RigidBody>  rb2);
    
    // collision with sphere, plane
    CollisionInfo testCollisionWith(std::shared_ptr<RigidSphere> rb2);
    CollisionInfo testCollisionWith(std::shared_ptr<RigidPlane> rb2);
    
    // To be implemented : collison with box
    // CollisionInfo testCollisionWith(std::shared_ptr<Box> rb2);
private:

    void init(float ar) {
        r = ar;
        matInertia = calcInertia();
    }
};

#endif