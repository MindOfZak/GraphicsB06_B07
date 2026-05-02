#ifndef __MESH_H__
#define __MESH_H__

#include <iostream>
#include <vector>

#include <glad/glad.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "Material.h"
#include "Texture.h"
#include "Spatial.h"


class Mesh {

public:
    // changed in LabA07
    // array of vertices and normals
    //std::vector< glm::vec3 > vertices; 
    std::vector<Vertex> vertices;
    // triangle vertex indices
    std::vector< unsigned int > indices;

    // added in LabA07
    Texture tex_diffuse;
    Texture tex_normal;

    // Material material;
    
    std::vector<GLuint> buffers;

    // this will be Material in the future
    GLuint shaderId;

    AABB bbox;

    // added in LabA 11
    bool bPicked = false;
    

public:

    std::unique_ptr<Spatial> pSpatial = nullptr;

    Mesh();
    ~Mesh();

    void initBuffer();
    void updateVertexBuffer();
    void draw(glm::mat4 matModel, glm::mat4 matView, glm::mat4 matProj);
    void setShaderId(GLuint sid);

    void updateBox();
    
    void initSpatial(bool useOctree, glm::mat4 mat);
    void setPicked(bool b) { bPicked = b; }

    // Zaks added this:
    // this is for snooker game
    glm::vec3 objectColour = glm::vec3(1.0f, 0.0f, 0.0f);
	void setColour(glm::vec3 colour) { objectColour = colour; }

};

#endif