#include "RigidBody.h"

#define LINEAR_DAMPING 0.9999
#define ANGULAR_DAMPING 0.999 

void RigidBody::integrateAcc(float dt)
{
    integrateLinearAcc(dt);
    integrateAngularAcc(dt);
}

void RigidBody::integrateVelocity(float dt)
{
    if (!bDynamic) return;

    integrateLinearVelocity(dt);
    integrateAngularVelocity(dt);
}

//=======================================
// Linear Dynamics

// TODO 1.1 : add acceleration from a force
void RigidBody::applyLinearForce(glm::vec3 force)
{
    if (! bDynamic) return;

    // TODO: 
    // linearAcc += ???;
    
}

// TODO 1.2 : acceleration integration
// update veloctiy by adding acceleration contributions
void RigidBody::integrateLinearAcc(float dt)
{
    if (! bDynamic) return;

    // TODO:
    // linearVel += ???;
    

    // Reset linear and angular acceleration.
    linearAcc = glm::vec3(0.0f);
}

// TODO 1.3 : velocity integration
// update object position by adding velocity contributions
void RigidBody::integrateLinearVelocity(float dt)
{
    if (! bDynamic) return;

    // TODO:
    // pos += ???;
    

    // reduce velocity a bit
    linearVel *= LINEAR_DAMPING;
}

// TODO 1.4 : apply collision response
// update velocity with impulse dividing by mass 
void RigidBody::applyLinearImpulse(glm::vec3 impulse)
{
    if (! bDynamic) return;

    // TODO
    // linearVel += ???;
    
}

//================================
// Angular Dynamics (Not required )
void RigidBody::applyAngularForce(glm::vec3 f, glm::vec3 r) {
    if (! bDynamic) return;

    glm::vec3 t = glm::cross(r, f);
    angularAcc += glm::inverse(matInertia) * t; 
}

void RigidBody::applyAngularImpulse(glm::vec3 i, glm::vec3 r) {
    if (! bDynamic) return;

    glm::vec3 torque = glm::cross(r, i);
    angularVel += glm::inverse(matInertia) * torque;
}


void RigidBody::integrateAngularAcc(float dt)
{
    // Integrate angular velocity
    angularVel += angularAcc * dt;

    // Reset linear and angular acceleration.
    angularAcc = glm::vec3(0.0f, 0.0f, 0.0f);
}

void RigidBody::integrateAngularVelocity(float dt)
{
    glm::quat velQuat(0.0f, angularVel.x, angularVel.y, angularVel.z);
    glm::quat dq = 0.5f * rotation * velQuat * dt;

    rotation += dq; 
    rotation = glm::normalize(rotation);

    angularVel *= ANGULAR_DAMPING;
}
