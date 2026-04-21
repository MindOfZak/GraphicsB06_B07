#ifndef __RIGID_OBJ_H__
#define __RIGID_OBJ_H__


#include <vector>
#include <memory>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Mesh.h"

struct CollisionInfo {
    bool bColliding;

    glm::vec3 pos;    // contact point
    glm::vec3 normal; // contact normal
    float peneDepth;  // penetration depth
};

class RigidBody
{
public:
    // for elastic collision response
    const float elasity = 0.8f;

    bool bDynamic = true;
    bool bUseGravity = true;
    
    // transforms
    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); // rotation
    glm::vec3 scale  = glm::vec3(1.0f, 1.0f, 1.0f);

    // Dynamic properities
    float mass = 1.0;
    glm::vec3 force = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 linearVel = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 linearAcc = glm::vec3(0.0f, 0.0f, 0.0f);

    // angular velocity and acceleration
    glm::vec3 angularVel = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 angularAcc = glm::vec3(0.0f, 0.0f, 0.0f);

    glm::mat3 matInertia = glm::mat3(1.0f);

    std::shared_ptr<Mesh> mesh;

    void applyLinearForce(glm::vec3 force);    // To be completed in RigidBody.cpp
    void applyLinearImpulse(glm::vec3 impulse); // To be completed in RigidBody.cpp

    // for angular effects
    void applyAngularForce(glm::vec3 f, glm::vec3 r);
    void applyAngularImpulse(glm::vec3 i, glm::vec3 r);
    virtual glm::mat3 calcInertia() { return glm::mat3(1.0f); }

    void integrateAcc(float dt);
    void integrateLinearAcc(float dt);  // To be completed in RigidBody.cpp
    void integrateAngularAcc(float dt);

    void integrateVelocity(float dt);
    void integrateLinearVelocity(float dt);  // To be completed in RigidBody.cpp
    void integrateAngularVelocity(float dt);

    void setDynamic(bool b) {  bDynamic = b; }

    void setUseGravity(bool b) { bUseGravity = b;  }

    void setMesh(std::shared_ptr<Mesh> m) { mesh = m; }

    void setMass(float m) { mass = m; }

    void setPosition(glm::vec3 apos) { pos = apos; }

    void setVelocity(glm::vec3 vel) {  linearVel = vel; }

    //void setRotation(RigidBody& rb, glm::quat q);
    //void setRotation(RigidBody &rb, float angleDegrees, glm::vec3 axis);

    void draw(glm::mat4 matView, glm::mat4 matProj) {
        glm::mat4 matTrans = glm::translate(glm::mat4(1.0f), pos);
        glm::mat4 matRot    = glm::mat4_cast(rotation);
        glm::mat4 matScale  = glm::scale(glm::mat4(1.0f), scale);

        glm::mat4 modelTrans = matTrans * matRot *  matScale;

        mesh->draw(modelTrans, matView, matProj);
    }

    virtual CollisionInfo testCollisionWith(std::shared_ptr<RigidBody> rb2) 
    {
        //std::cout << "RigidBody::testCollisioinWith" << std::endl;
        CollisionInfo info;
        info.bColliding = false;
        return info;
    }

};

#endif