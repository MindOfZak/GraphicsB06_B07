#include <iostream>

#include <glad/glad.h>

#include "Mesh.h"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

//#define STB_IMAGE_IMPLEMENTATION
//#include <stb/stb_image.h>

#include "Grid.h"
#include "Octree.h"

Mesh::Mesh()
{
    bbox.min = glm::vec3(0, 0, 0);
    bbox.max = glm::vec3(1, 1, 1);
}

Mesh::~Mesh()
{

}

void Mesh::updateBox() {
    glm::vec3 min(FLT_MAX, FLT_MAX, FLT_MAX);
    glm::vec3 max(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    for (int i = 0; i < vertices.size(); i++)
    {
        Vertex v = vertices[i];
        glm::vec3 pos = v.pos;

        if (min.x > pos.x)
            min.x = pos.x;
        if (min.y > pos.y)
            min.y = pos.y;
        if (min.z > pos.z)
            min.z = pos.z;

        if (max.x < pos.x)
            max.x = pos.x;
        if (max.y < pos.y)
            max.y = pos.y;
        if (max.z < pos.z)
            max.z = pos.z;
    }
}

void Mesh::initSpatial(bool useOctree, glm::mat4 mat)
{
    if (useOctree)
        pSpatial = std::make_unique<Octree>();
    else
        pSpatial = std::make_unique<Grid>(glm::ivec3(16));
    
    pSpatial->Build(vertices, indices, mat);
}


void Mesh::initBuffer()
{
    updateBox();

    // create VAO
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    buffers.push_back(vao);

    GLuint vertBufID;
    glGenBuffers(1, &vertBufID);
    glBindBuffer(GL_ARRAY_BUFFER, vertBufID);
    
    // create and bind index buffer
    GLuint idxBufID;
    glGenBuffers(1, &idxBufID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxBufID);
    
    // changed in LabA07 
    // set buffer data to triangle vertex and setting vertex attributes
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0] /*vertices.data()*/, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

    // set normal attributes
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (sizeof(float) * 3));

    // added in LabA07: Adding texture coord attribute
    // vertex texture coords
    glEnableVertexAttribArray(2);
    // the second parameter: 2 coordinates (tx, ty) per texture coord	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

    // set buffer data for triangle index
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Mesh::updateVertexBuffer()
{
    glBindVertexArray(buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0] /*vertices.data()*/, GL_DYNAMIC_DRAW);
    
    //glEnableVertexAttribArray(0);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

    // set normal attributes
    //glEnableVertexAttribArray(1);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 3));
}

void Mesh::setShaderId(GLuint sid) {
    shaderId = sid;
}


// all drawings come here
void Mesh::draw(glm::mat4 matModel, glm::mat4 matView, glm::mat4 matProj)
{
    // 1. Bind the correct shader program
    glUseProgram(shaderId);

    //std::cout << "shader: " << shaderId << std::endl;

    // 2. Set the appropriate uniforms for each shader
    // set the modelling transform  
    GLuint model_loc = glGetUniformLocation(shaderId, "model" );
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, &matModel[0][0]);

    // set view matrix
    GLuint view_loc = glGetUniformLocation(shaderId, "view" );
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, &matView[0][0]);

    // set projection transforms
    glm::mat4 mat_projection = matProj;
    GLuint projection_loc = glGetUniformLocation( shaderId, "projection" );
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, &mat_projection[0][0]);

    // Diffuse texture
    if (tex_diffuse.id > 0) {
        tex_diffuse.bindTexture();
        GLint textureLoc = glGetUniformLocation(shaderId, "textureMap");
        // always use texture unit 0 for diffuse texture
        glUniform1i(textureLoc, 0);
    }

    // =====================================================
    // LabA 11 Spatial Data Structures
    glUniform1i(glGetUniformLocation(shaderId, "bPicked"), bPicked);

    // 3. Bind the corresponding model's VAO
    glBindVertexArray(buffers[0]);

    // 4. Draw the model
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    // 5. Unset vertex buffer
    glBindVertexArray(0);
}