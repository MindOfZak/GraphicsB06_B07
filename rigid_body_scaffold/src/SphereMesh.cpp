#include "SphereMesh.h"

/*
SphereMesh::SphereMesh() : res_phi(10), res_theta(10), radius(1.0f)
{
	create_sphere();
}
*/

SphereMesh::SphereMesh(const int res_p, const int res_t, float r) : 
                res_phi(res_p), res_theta(res_t), radius(r)
{
	create_sphere();
    initBuffer();
}

SphereMesh::~SphereMesh()
{

}

void SphereMesh::create_sphere() {

    float PI = glm::pi<float>();

	stepPhi = 2.0 * PI / res_phi;
	stepTheta = PI / res_theta;

    Vertex v;
    int idx = 0;

	// Top piece
	glm::vec3 top = glm::vec3(0, 1, 0) ;
    v.pos = radius * top;
    v.normal = top;
    v.uv = glm::vec2(0, 0);
    vertices.push_back(v);
    //indices.push_back(idx);

    glm::vec3 pos = glm::vec3(0.0);
    float phi = 0;

	for (int j = 0; j < res_phi; j++) {
        
        idx++;

		pos.y = cos(stepTheta);
        // current radius
        float cr = sin(stepTheta);

        pos.x = cr * cos(stepPhi * j);
        pos.z = cr * sin(stepPhi * j);

        v.pos = radius * pos;
        v.normal = pos;
        v.uv = glm::vec2(j / (float) res_phi, 1 / (float) res_theta);
        vertices.push_back(v);
        
        
        if (j > 0) {
            indices.push_back(0);
            indices.push_back(idx - 1);
            indices.push_back(idx);
        } else {
            // j = 0
            indices.push_back(0);
            indices.push_back(res_phi);
            indices.push_back(1);
        }

        phi += stepPhi;
	}

	// Middle piece
	for (int i = 2; i < res_theta; i++) 
    {
        phi = 0;
        pos.y = cos(i * stepTheta);
        float cr = sin(i * stepTheta);

		for (int j = 0; j < res_phi; j++) 
        {
            idx ++;

            pos.x = cr * cos(phi);
            pos.z = cr * sin(phi);
            
            v.pos = radius * pos;
            v.normal = pos;
            v.uv = glm::vec2(j / (float) res_phi, 1 / (float) res_theta);
            vertices.push_back(v);

            if (j > 0) {
                indices.push_back(idx - 1 - res_phi);
                indices.push_back(idx - 1);
                indices.push_back(idx);

                indices.push_back(idx - res_phi);
                indices.push_back(idx - 1 - res_phi);
                indices.push_back(idx);
            } else {
                // j = 0
                indices.push_back(idx - res_phi);
                indices.push_back(idx - 1);
                indices.push_back(idx);

                indices.push_back(idx - 1);
                indices.push_back(idx + res_phi - 1);
                indices.push_back(idx);
            }

            phi += stepPhi;
		}
	}

	// Bottom piece
	glm::vec3 bottom = glm::normalize(glm::vec3(0, -1, 0));
    v.pos = radius * bottom;
    v.normal = bottom;
    v.uv = glm::vec2(0, 1.0);
    vertices.push_back(v);

    phi = 0;

    for (int j = 0; j < res_phi; j++) {
        idx++;
/*
		pos.y = cos(PI - stepTheta);
        float cr = sin(PI - stepTheta);
        pos.x = cr * cos(phi);
        pos.z = cr * sin(phi);

        v.pos = radius * pos;
        v.normal = pos;
        v.uv = glm::vec2(j / (float) res_phi, (1 - 1 / (float) res_theta));
        vertices.push_back(v);
*/
        if (j > 0) {
            indices.push_back(idx - res_phi);
            indices.push_back(idx - res_phi - 1);
            indices.push_back(res_phi * ( res_theta - 1) + 1);
        } else {
            // j = 0
            indices.push_back(res_phi * ( res_theta - 1) );
            indices.push_back(idx - res_phi);
            indices.push_back(res_phi * ( res_theta - 1) + 1);
        }

        //phi += stepPhi;

	}

    //std::cout << vertices.size() << std::endl;
    //std::cout << indices.size() << std::endl;

}