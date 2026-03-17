#ifndef __MESH_H__
#define __MESH_H__

#include <iostream>
#include <vector>

#include <glad/glad.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <assimp/material.h>
#include <assimp/scene.h> 
#include <map> 
#include "Bone.h" 

#define MAX_BONE_INFLUENCE 4  


inline glm::mat4 ConvertMatrixToGLMFormat(const aiMatrix4x4& from);

struct Vertex {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 texCoord;
    
    // for normal mapping
    // tangent
    glm::vec3 tangent;
    // bitangent
    glm::vec3 bitangent;

    // bone info added
    //bone indexes which will influence this vertex
    int boneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float weights[MAX_BONE_INFLUENCE];

};

// added in LabA07
struct Texture {
    GLuint id;
    std::string type;
    //std::string path;
};

// added in LabA07
struct Material {
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 ambient;
    float shininess;
};


class Mesh {

protected:

    // array of vertices and normals
    std::vector<Vertex> vertices;

    // triangle vertex indices
    std::vector< unsigned int > indices;

    std::vector<Texture> textures;
    std::vector<Texture> normals;

    Material material;
    
    std::vector<GLuint> buffers;

    // this will be Material in the future
    GLuint shaderId;

    void initBuffer();

    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName, std::string dir);
    unsigned int loadTextureAndBind(const char* path, const std::string& directory);
    Material Mesh::loadMaterial(aiMaterial* mat);

    std::map<std::string, BoneInfo> boneInfoMap; //
    int boneCounter = 0;

    auto& GetBoneInfoMap() { return boneInfoMap; }
    int& GetBoneCount() { return boneCounter; }

    void SetVertexBoneDataToDefault(Vertex& vertex);
    void SetVertexBoneData(Vertex& vertex, int boneID, float weight);
    void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);

public:
    Mesh();
    ~Mesh();

    void init(std::string path, GLuint shaderId);
    void loadModel(std::string path);

    void setShaderId(GLuint sid);
    
    void draw(glm::mat4 matModel, glm::mat4 matView, glm::mat4 matProj);

    std::map<std::string, BoneInfo>& getBoneInfoMap() { return boneInfoMap; }
    int getBoneCount() { return boneCounter; }
};

#endif