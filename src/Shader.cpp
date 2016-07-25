/////////////////////////////////////////////////////////////////////////////
// Name:        Shader.cpp
// Implementation of OpenGL Shader class
/////////////////////////////////////////////////////////////////////////////
#include <iostream>

// Include GLEW
#include </usr/include/GL/glew.h>

// Include GLFW
#include </usr/local/include/GLFW/glfw3.h>

#include "../include/Shader.h"

static int shaderID = 1;

Shader::Shader(const GLchar* vsource, const GLchar* fsource) {
	vshadesource = vsource;
	fshadesource = fsource;

	CreateProgram();

	std::cout << "In Shader constructor:  " << std::endl;
}

void Shader::CreateProgram() {
	std::cout << "in Shader CreateProgram()" << std::endl;

//     glewExperimental = GL_TRUE;
    GLenum err = glewInit();
// //    std::cout << "glewInit() call: " << glewInit() << std::endl;
// 	if(err != GLEW_OK) {
//         std::cout << "Error #: " << glGetError() << " str: " << glewGetErrorString(err) << std::endl;;

//         std::cout << "GLEW_OK: " << GLEW_OK << std::endl;

//         std::cout << "GL_NO_ERROR: " << GL_NO_ERROR << std::endl;
//         std::cout << "GL_INVALID_ENUM: " << GL_INVALID_ENUM << std::endl;
//         std::cout << "GL_INVALID_VALUE: " << GL_INVALID_VALUE << std::endl;
//         std::cout << "GL_OUT_OF_MEMORY: " << GL_OUT_OF_MEMORY << std::endl;
//         std::cout << "GL_STACK_UNDERFLOW: " << GL_STACK_UNDERFLOW << std::endl;
//         std::cout << "GL_STACK_OVERFLOW: " << GL_STACK_OVERFLOW << std::endl;       
//         std::cout << "Error in glewInit" << std::endl;
//     } else
//         std::cout << "GlewInit() okay" << std::endl;

//     std::cout << "2.in Shader CreateProgram()" << std::endl;
    //std::cout << "Drawing crosshairs" << std::endl;

    // Build and compile our shader program
    // Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    std::cout << "3. in Shader CreateProgram()" << std::endl;
    glShaderSource(vertexShader, 1, &vshadesource, NULL);
    glCompileShader(vertexShader);
    // Check for compile time errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    	std::cout << "4. in Shader CreateProgram()" << std::endl;
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fshadesource, NULL);
    glCompileShader(fragmentShader);
    // Check for compile time errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
std::cout << "3.  in Shader CreateProgram()" << std::endl;
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Link shaders
    m_ProgramNum = glCreateProgram();
    glAttachShader(m_ProgramNum, vertexShader);
    glAttachShader(m_ProgramNum, fragmentShader);
    glLinkProgram(m_ProgramNum);
    std::cout << "Shader number: " << m_ProgramNum << std::endl;
    // Check for linking errors
    glGetProgramiv(m_ProgramNum, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_ProgramNum, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::Load() {

}

void Shader::Unload() {

}