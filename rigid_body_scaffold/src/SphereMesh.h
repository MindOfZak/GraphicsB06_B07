#ifndef __SPHERE_MESH_H__
#define __SPHERE_MESH_H__

#include "Mesh.h"

class SphereMesh : public Mesh
{
public:
    //static float PI = glm::pi<float>();
	// SphereMesh();
	SphereMesh(const int res_p = 10, const int res_t = 10, float r = 1.0f);
	~SphereMesh();

	// class methods
	void create_sphere();

protected:

private:
	const int res_phi, res_theta;
	float radius;
	float stepPhi, stepTheta;

};


#endif