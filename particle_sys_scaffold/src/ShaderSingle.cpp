#include "ShaderSingle.h"


#include <iostream>
#include <fstream>
#include <string>


using namespace std ;

void ShaderSingle::read_source(const char * filename)  {
    source_code = textFileRead(filename);
}

void ShaderSingle::compile()  {
    shader_id = glCreateShader(type);
    const GLchar * cstr = source_code.c_str() ; // convert source to const GLchar *
    glShaderSource( shader_id, 1, &cstr, NULL) ;
    glCompileShader( shader_id );
    glGetShaderiv ( shader_id, GL_COMPILE_STATUS, & compiled_status ) ;
    if (! compiled_status) {
        cout << "Shader Error";
        shadererrors( shader_id) ;
        throw 3 ;
    }

    /*
    program_id = glCreateProgram() ;
    glAttachShader(program_id, shaderObj) ;
    glLinkProgram(program_id) ;
    glGetProgramiv(program_id, program_id, &bLinked) ;

    if (bLinked){
        glDetachShader( program_id, shaderObj );
        glDetachShader( program_id, shaderObj );
        glDeleteShader( shaderObj );
    }else{
        programerrors(program_id) ;
        throw 4 ;
    }
    */
}

// Below are helper functions

string ShaderSingle::textFileRead (const char * filename) {
	string str, ret = "" ; 
	ifstream in ; 
	in.open(filename) ; 
	if (in.is_open()) {
		::getline(in, str) ; 
		while (in) {
			ret += str + "\n" ; 
			::getline(in, str) ; 
		}
		//    cout << "Shader below\n" << ret << "\n" ; 
		return ret ; 
	}
	else {
		cerr << "Unable to Open File " << filename << "\n" ; 
		throw 2 ; 
	}
}

void ShaderSingle::programerrors (const GLint program) {
	GLint length ; 
	GLchar * log ; 
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length) ; 
	log = new GLchar[length+1] ;
	glGetProgramInfoLog(program, length, &length, log) ; 
	cout << "Compile Error, Log Below\n" << log << "\n" ; 
	delete [] log ; 
}
void ShaderSingle::shadererrors (const GLint shader) {
	GLint length ; 
	GLchar * log ; 
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length) ; 
	log = new GLchar[length+1] ;
	glGetShaderInfoLog(shader, length, &length, log) ; 
	cout << "Compile Error, Log Below\n" << log << "\n" ; 
	delete [] log ; 
}
