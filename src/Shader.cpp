/////////////////////////////////////////////////////////////////////////////
// Name:        Shader.cpp
// Implementation of OpenGL Shader class
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

// Include GLEW
#include </usr/include/GL/glew.h>

// Include GLFW
#include </usr/local/include/GLFW/glfw3.h>

#include "../include/Shader.h"

static int shaderID = 1;

//Shader::Shader(const char* vsource, const char* fsource) {
Shader::Shader(std::string vshadepath, std::string fshadepath) {

    std::cout << "---- shader constructor" << std::endl;

    vertexPath = vshadepath;
    fragmentPath = fshadepath;
    // vertexPath = "shaders/default_shader.vert";
    // fragmentPath = "shaders/default_shader.frag";

	CreateProgram();
}

void Shader::CreateProgram() {
	std::cout << "in Shader CreateProgram()" << std::endl;

    GLenum err = glewInit();


    std::string strVShader = LoadFromFile(vertexPath.c_str());
    std::string strFShader = LoadFromFile(fragmentPath.c_str());

    vshadesource = strVShader.c_str();
    fshadesource = strFShader.c_str();

    // Build and compile our shader program
    // Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vshadesource, NULL);
    glCompileShader(vertexShader);
    // Check for compile time errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

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

    std::cout << "vertexShader num:" << vertexShader << std::endl;
    std::cout << "fragmentShader num:" << fragmentShader << std::endl;
    std::cout << "Shader program number: " << m_ProgramNum << std::endl;

    // Check for linking errors
    glGetProgramiv(m_ProgramNum, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_ProgramNum, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return;
}

std::string Shader::LoadFromFile(std::string strFile) {
    // Open the file passed in
    std::ifstream fin(strFile.c_str());

    // Make sure we opened the file correctly
    if ( !fin )
    {
        printf("\nError opening shader file: %s", strFile.c_str());
        return "";
    }
    std::string strLine = "";
    std::string strText = "";

    // Go through and store each line in the text file within a "string" object
    while ( getline(fin, strLine) )
    {
        strText = strText + "\n" + strLine;
    }

    // Close our file
    fin.close();

//    std::cout << "strText: " << strText << std::endl;

    // Return the text file's data
    return strText;
}

void Shader::Unload() {

}