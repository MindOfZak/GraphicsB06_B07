#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <unordered_map>
#include <memory>
#include "ShaderProgram.h"
#include "Texture.h"


// try to move Material from traditional OpenGL
// to shader based
class Material {
public:
    // parameters for 
    // tradtional phong and blinn-phong models 
    // for compatibility with existing programs
    glm::vec3 Diffuse;
    glm::vec3 Specular;
    glm::vec3 Ambient;
    float Shininess;

private:
    std::shared_ptr<ShaderProgram> shaderProgram = nullptr;

    std::unordered_map<std::string, GLint> intParams;
    std::unordered_map<std::string, float> floatParams;
    std::unordered_map<std::string, glm::vec2 > vec2Params;
    std::unordered_map<std::string, glm::vec3 > vec3Params;
    std::unordered_map<std::string, glm::vec4 > vec4Params;
    std::unordered_map<std::string, glm::mat4 > mat4Params;

    std::unordered_map<std::string, std::shared_ptr<Texture> > textures;

public:
    // old style material for compatibility
    Material()  {}

    // material binded with shaders
    Material(std::shared_ptr<ShaderProgram> shader) 
    { 
        // set the shader program
        setShaderProgram(shader);
    }

    // --- Shader ---
    void setShaderProgram(std::shared_ptr<ShaderProgram> shader)
    {
        shaderProgram = shader;
        // Bind shader program
        shaderProgram->link();
    }

    /*
    std::shared_ptr<ShaderProgram> getShader() const
    {
        return shaderProgram;
    }
    */

    // --- Uniform Parameters ---
    void setInt(const std::string& name, GLuint value)
    {
        intParams[name] = value;
    }

    void setFloat(const std::string& name, float value)
    {
        floatParams[name] = value;
    }

    void setVec2(const std::string& name, glm::vec2 v) // assume vec3[3]
    {
        vec2Params[name] = v;
    }

    void setVec3(const std::string& name, glm::vec3 v) // assume vec3[3]
    {
        vec3Params[name] = v;
    }

    void setVec4(const std::string& name, glm::vec4 v) // assume vec3[3]
    {
        vec4Params[name] = v;
    }

    void setMat4(const std::string& name, glm::mat4 v) // assume 16 floats
    {
        mat4Params[name] = v;
    }

    // --- Textures ---
    void setTexture(const std::string& name, std::shared_ptr<Texture> texture)
    {
        textures[name] = texture;
    }

    // --- Bind material before drawing ---
    void bind()
    {
        if ( nullptr == shaderProgram)  return;

        // Bind shader program
        shaderProgram->link();
        
        // bind the shader
        shaderProgram->use();

        // Upload ints
        for (auto& [name, value] : intParams)
            shaderProgram->setInt(name, value);

        // Upload floats
        for (auto& [name, value] : floatParams)
            shaderProgram->setFloat(name, value);

        // Upload vec3s
        for (auto& [name, vec] : vec3Params)
           shaderProgram->setVec3(name, vec);

        // Upload matrices
        for (auto& [name, mat] : mat4Params)
            shaderProgram->setMat4(name, mat);

        // Bind textures
        int textureUnit = 0;
        for (auto& [name, tex] : textures)
        {
            tex->bindTexture(textureUnit);
            shaderProgram->setInt(name, textureUnit);
            textureUnit++;
        }
    }

    void unbind()
    {
        // nothing to unbind
        if ( nullptr == shaderProgram) return;

        // Unbind textures
        // Unbind all owned objects.
        int textureUnit = 0;
        for (auto& [name, tex] : textures) 
        {
            tex->UnbindTexture(textureUnit);
            textureUnit++;
        }

        shaderProgram->unUse();
    }
};


#endif



