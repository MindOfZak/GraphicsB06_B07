#include "RigidSphere.h"

glm::mat3 RigidSphere::calcInertia()
{
    float d = (2.0f / 5.0f) * mass * r * r;

    return glm::mat3(
        d, 0.0f, 0.0f,
        0.0f, d, 0.0f,
        0.0f, 0.0f, d
    );
}

CollisionInfo RigidSphere::testCollisionWith(std::shared_ptr<RigidBody> rb2) {
    std::shared_ptr<RigidSphere> sphere =
               std::dynamic_pointer_cast<RigidSphere> (rb2);
    if (sphere != std::shared_ptr<RigidSphere>()) {
        return testCollisionWith(sphere);
    }

    std::shared_ptr<RigidPlane> plane =
        std::dynamic_pointer_cast<RigidPlane> (rb2);
    if (plane != std::shared_ptr<RigidPlane>()) {
        return testCollisionWith(plane);
    }
    
    return RigidBody::testCollisionWith(rb2);
}

// TODO 4: implement sphere-sphere collision detection
// return the CollisionInfo: contact point, contact normal, penetration depth 
CollisionInfo RigidSphere::testCollisionWith(std::shared_ptr<RigidSphere> rb2) {

    CollisionInfo info;
    // no collision by default
    info.peneDepth = 0.0f;
    
    // vector from sphere 2 center to sphere 1    
    glm::vec3 vPos = pos - rb2->pos;

    // TODO 4.1: distance between two sphere centre
    float dist = 0.0f;
    //dist = ???;

    // TODO 4.2: contact surface normal is the normalisation of vPos
    //info.normal = ???;

    // TODO 4.3: penetration depth
    info.peneDepth = r + rb2->r - dist;
    // info.peneDepth = ???;

    if (info.peneDepth > 0)  {
        // two spheres are colliding
        info.bColliding = true;

        // set the contact point to the middle of the penetration
        info.pos = (pos + rb2->pos) * 0.5f;
    }
    else {
        // two spheres are apart
        info.bColliding = false;
    }
    
    return info; 

}


// for a ground, the plane is static
CollisionInfo RigidSphere::testCollisionWith(std::shared_ptr<RigidPlane> rb2)
{
    CollisionInfo info;
    
    info.normal = rb2->normal; // normalisation

    // origin of sphere to plane distance
    float dist = rb2->pointDist(pos);
    float depth = r - dist;
    info.peneDepth = depth;

    
    // fix the above ceiling bug temporarily
    if ((depth > 0) && (depth < r)) {
        // Impulse-based response:
        info.bColliding = true;

        // contact point 
        info.pos = pos - (dist + depth * 0.5f) * rb2->normal;
    }
    else {
        info.bColliding = false;
    }
    
    return info; 
}