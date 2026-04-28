#ifndef __SHADER_SINGLE_H__
#define __SHADER_SINGLE_H__

#include <string>
#include "glad/glad.h"


class ShaderSingle {
private:
    GLuint shader_id;      // intermediate shader object
    GLint compiled_status = 0;    // compile status
    GLint bLinked = GL_FALSE;         // link status

public:
    //GLuint program_id;  // the shader program id
    GLenum type;    // to support compute shaders

    std::string source_code;   // source code

    // default constructor
    ShaderSingle() {} 

    ShaderSingle(const char * filename, GLenum shader_type) {
        type = shader_type;
        read_source(filename);
        compile();
    } 

    void read_source(const char * filename);
    void compile();

    GLint getShaderCompileStatus() {return compiled_status;}
    GLint getLinkStatus() {return bLinked;}

    GLenum getShaderType() { return type; }
    GLuint getShaderId() { return shader_id; }

    
private:
    // Helper functions
    static std::string textFileRead(const char * filename );
    static void programerrors(const GLint program);
    static void shadererrors(const GLint shader);
};


#endif