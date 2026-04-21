#include "MeshFactory.h"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>


std::shared_ptr<Mesh> MeshFactory::importAssimp(std::string path)
{
    std::shared_ptr<Mesh> outMesh = std::make_shared<Mesh>();

    Assimp::Importer importer;
    // LabA07 change: aiProcess_FlipUVs
    // TODO: can add triangulate and create bitangents for normal mapping
    const aiScene* scene = importer.ReadFile(path, aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs);
    if (NULL != scene) {
        std::cout << "load model successful" << std::endl;
    } else {
        std::cout << "load model failed" << std::endl;
    }

    Vertex v;

    // at the moment we only handle one mesh
    for (int i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[i];
        
        // read vertex position and normals
        int nVertex = mesh->mNumVertices;
        // std::cout << mesh->mNumVertices << std::endl;

        for (int j = 0; j < nVertex; j++)
        {
            glm::vec3 pos; 
            pos.x = mesh->mVertices[j].x;
            pos.y = mesh->mVertices[j].y;
            pos.z = mesh->mVertices[j].z; 
            v.pos = pos;

            glm::vec3 normal;
            normal.x = mesh->mNormals[j].x;
            normal.y = mesh->mNormals[j].y;
            normal.z = mesh->mNormals[j].z;
            v.normal = normal;

            if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][j].x; 
                vec.y = mesh->mTextureCoords[0][j].y;
                v.uv = vec;
            }
            else {
                //std::cout << "tex coord zero" << std::endl;
                v.uv = glm::vec2(0.0f, 0.0f);
            }

            outMesh->vertices.push_back(v);
        }

		int nFaces = mesh->mNumFaces;
        for (int j = 0; j < nFaces; j++ )
        {
            const aiFace& face = mesh->mFaces[j];
            for (int k = 0; k < 3; k++)
            {
                outMesh->indices.push_back(face.mIndices[k]); 
            }
        }
    }

    // added in LabA07
    // loading material and texture, at the moment we only deal with one material/texture
    aiMesh* mesh = scene->mMeshes[0];

    if (NULL != mesh && mesh->mMaterialIndex > 0)
    {
        // get directory of the model
        std::string dir = "";
        const size_t last_slash_idx = path.rfind('/');
        if (std::string::npos != last_slash_idx) {
            dir = path.substr(0, last_slash_idx);
        }

        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        loadAssimpMaterialTextures(outMesh, material,aiTextureType_DIFFUSE, "texture_diffuse", dir);
        
        // this->material = loadMaterial(material);

        // we don't deal with specular maps
        //std::vector<Texture> specularMaps = loadMaterialTextures(material, 
        //                                    aiTextureType_SPECULAR, "texture_specular", dir);
        //textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    outMesh->initBuffer();
    
    // for debugging
    std::cout << "numVertex: " <<  outMesh->vertices.size() << std::endl;
    std::cout << "numIndex: " << outMesh->indices.size() << std::endl;

    return outMesh;
}

void MeshFactory::loadAssimpMaterialTextures(std::shared_ptr<Mesh> pMesh, aiMaterial *mat, aiTextureType type, std::string typeName, std::string dir)
{
    // actually, we only support one texture
    int nTex = mat->GetTextureCount(type);

    // we only load one texture
    for(unsigned int i = 0; i < 1 ; i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::string filepath = dir + '/' + std::string(str.C_Str());

        if (aiTextureType_DIFFUSE == type) {
            pMesh->tex_diffuse.loadTexture(filepath);
            //pMesh->tex_diffuse.type = typeName;
        }
        else if ((aiTextureType_HEIGHT == type) 
            || (aiTextureType_NORMALS == type)) {
            pMesh->tex_normal.loadTexture(filepath);
            //pMesh->tex_normal.type = typeName;
        }
    }
}  

Material MeshFactory::loadAssimpMaterial(aiMaterial* mat) 
{
    Material material;
    aiColor3D color(0.f, 0.f, 0.f);
    float shininess;

    mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    material.Diffuse = glm::vec3(color.r, color.b, color.g);

    mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
    material.Ambient = glm::vec3(color.r, color.b, color.g);

    mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
    material.Specular = glm::vec3(color.r, color.b, color.g);

    mat->Get(AI_MATKEY_SHININESS, shininess);
    material.Shininess = shininess;

    return material;
}

std::shared_ptr<PlaneMesh> MeshFactory::createPlane(int direction, float width, float length, int resWid, int resLen, float height, std::string texPath) {
    std::shared_ptr<PlaneMesh> plane 
            = std::make_shared<PlaneMesh>(direction, width, length, resWid, resLen, - width/ 2.0, -length / 2.0, height);

    if (texPath != "")
        plane->tex_diffuse.loadTexture(texPath);
    else
        plane->tex_diffuse.id = -1;

    plane->initBuffer();

    return plane;
}