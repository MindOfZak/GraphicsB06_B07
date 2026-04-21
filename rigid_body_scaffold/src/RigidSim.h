#ifndef __RIGID_SIM_H__
#define __RIGID_SIM_H__

#include <vector>
#include <memory>

#include "RigidBody.h"

class RigidSim 
{
public:

    // global constants
    const glm::vec3 GRAVITY = glm::vec3(0.0f, -9.80665, 0.0f);

    const float LINEAR_DAMPING = 0.999;
    const float ANGULAR_DAMPING = 0.999;

    // list of rigid body objects
    std::vector<std::shared_ptr<RigidBody> > objList;

    // settings
    bool bPlay = false;
    bool bUseAngular = false;
    
    RigidSim() {}

    // add rigid body object
    void add(std::shared_ptr<RigidBody> obj) { objList.push_back(obj); }

    // update every time step
    void tick(float dt);

    // draw rigid body meshes
    void draw();

    // update velocit after collision detected
    void collisionResponse(std::shared_ptr<RigidBody> a, std::shared_ptr<RigidBody> b, CollisionInfo info);

    void setPlaySim(bool b) {
        bPlay = b;
    }
    
    void setUseAngular(bool b) {
        bUseAngular = b;
    }

    void clear() {
        objList.clear();
        bPlay = false;
    }
};

#endif 
