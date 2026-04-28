#ifndef __SHADER_PROGRAM_H__
#define __SHADER_PROGRAM_H__

#include "ShaderSingle.h"

#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

// Wraps opengl shader program functionality
class ShaderProgram
{

private:
    // These shader objects wrap the functionality of loading and compiling shaders from files.
    std::shared_ptr<ShaderSingle> computeShader = nullptr;
    std::shared_ptr<ShaderSingle> vertexShader = nullptr;
    //Shader* m_geometryShader = nullptr;
    std::shared_ptr<ShaderSingle> fragShader = nullptr;

    // GL index for shader program
    GLuint shaderProgram;

    // Keep track of if the program has been built and only build when needed
    // bool m_programBuilt = false;

    bool bLinked = false;


public:
    ShaderProgram();
    ~ShaderProgram();
    GLuint getGLShaderProgram()  { return shaderProgram; }
    void attachShader(std::shared_ptr<ShaderSingle> shader);
    
    void link();

    void use() const {   glUseProgram(shaderProgram); }
    void unUse() { glUseProgram(0); }

    void setFloat(const std::string& name, float value) const;
    void setInt(const std::string& name, int value) const;
    void setVec2(const std::string& name, glm::vec2 value) const;
    void setVec3(const std::string& name, glm::vec3 value) const;
    void setVec4(const std::string& name, glm::vec4 value) const;
    void setMat4(const std::string& name, glm::mat4 value) const;
};

#endif