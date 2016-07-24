/////////////////////////////////////////////////////////////////////////////
// Name:        DrawObject.cpp
// Implementation of DrawingObject classes
/////////////////////////////////////////////////////////////////////////////

#include <iostream>

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#if !wxUSE_GLCANVAS
    #error "OpenGL required: set wxUSE_GLCANVAS to 1 and rebuild the library"
#endif


// Include GLEW
#include </usr/include/GL/glew.h>

// Include GLFW
#include </usr/local/include/GLFW/glfw3.h>

#include "../include/cube.h"
#include "../include/Geometry.h"
#include "../include/TestGLCanvas.h"
#include "../include/DrawObject.h"
#include "../include/Shader.h"

const GLchar* myvertexShaderSource = "#version 300 es\n"
    "layout (location = 0) in vec3 position;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
    "}\0";
const GLchar* myfragmentShaderSource = "#version 300 es\n"
    "out highp vec4 color;\n"
    "void main()\n"
    "{\n"
    "color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";






// default constructor
VertexData::VertexData() {
	VertexData(0.0f, 0.0f, 0.0f);
}

VertexData::VertexData(GLfloat x_pt, GLfloat y_pt, GLfloat z_pt){
	x = x_pt;
	y = y_pt;
	z = z_pt;
}

// default constructor
DrawObject::DrawObject(){
	std::cout << "DrawObject constructor" << std::endl;
}

// default constructor
Crosshairs::Crosshairs(){
	std::cout << "Crosshair constructor" << std::endl;
 //   Shader *myShader = new Shader(myvertexShaderSource, myfragmentShaderSource);
}

void Crosshairs::Render(TestGLCanvas *orig_canvas)
{
	std::cout << "Render crosshair now" << std::endl;
	    glewInit();
    
    //std::cout << "Drawing crosshairs" << std::endl;

    // Build and compile our shader program
    // Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &myvertexShaderSource, NULL);
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
    glShaderSource(fragmentShader, 1, &myfragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // Check for compile time errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Link shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    int ht = orig_canvas->CANVAS_HEIGHT;
    int width = orig_canvas->CANVAS_WIDTH;
    int curr_x = orig_canvas->getCurrMousePos()->getX();
    int curr_y = orig_canvas->getCurrMousePos()->getY();

    GLfloat screen_x = (2.0 * curr_x / width - 1.0);
    GLfloat screen_y = (1.0 - 2.0 * curr_y / ht);

    // Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices[] = {
         -1.0f, screen_y, 0.0f,     // Left  
          1.0f, screen_y, 0.0f,     // Right 
          screen_x, 1.0f, 0.0f,     // Top 
          screen_x, -1.0f, 0.0f     // Bot 
    };

    int num_verts = sizeof(vertices) / (3 * sizeof(GLfloat));

    // std::cout << curr_x << " , " << curr_y << std::endl;
    // std::cout << num_verts << std::endl;

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

    // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
    glfwPollEvents();

    // Render

    // Draw our first triangle
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, num_verts);
    glBindVertexArray(0);

    // Properly de-allocate all resources once they've outlived their purpose
    glUseProgram(0); // stop using a shader program
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Crosshairs::AddVertex(){
	std::cout << "Add vertex for crosshair" << std::endl;
}

void Crosshairs::CreateProgram(){
    std::cout << "In Crosshairs Create Program: "  << std::endl;
}

