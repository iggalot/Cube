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

// Include GLM
#include </usr/include/glm/glm.hpp>
#include </usr/include/glm/gtc/matrix_transform.hpp>
#include </usr/include/glm/gtc/type_ptr.hpp>
// #include </usr/include/glm/ext.hpp>  // for displaying vectors
#include </usr/include/glm/gtx/string_cast.hpp>  // for displaying vectors


#include "../include/cube.h"
#include "../include/Geometry.h"
#include "../include/TestGLCanvas.h"
#include "../include/DrawObject.h"
#include "../include/Shader.h"
#include "../include/Camera.h"

// const GLchar* myvertexShaderSource = "#version 300 es\n"
//     "layout (location = 0) in vec3 position;\n"
//     "void main()\n"
//     "{\n"
//     "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
//     "}\0";
// const GLchar* myfragmentShaderSource = "#version 300 es\n"
//     "out highp vec4 color;\n"
//     "void main()\n"
//     "{\n"
//     "color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
//     "}\n\0";

void displayMat4(glm::mat4 mat) {
    const float *pSource = (const float*)glm::value_ptr(mat);
    std::cout << "===================================" << std::endl;
    std::cout << pSource[0] << " , " << pSource[1] << " , " << pSource[2] << " , " << pSource[3] << std::endl;
    std::cout << pSource[4] << " , " << pSource[5] << " , " << pSource[6] << " , " << pSource[7] << std::endl;
    std::cout << pSource[8] << " , " << pSource[9] << " , " << pSource[10] << " , " << pSource[11] << std::endl;
    std::cout << pSource[12] << " , " << pSource[13] << " , " << pSource[14] << " , " << pSource[15] << std::endl;
    std::cout << "===================================" << std::endl;
}

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
    shader = NULL;
    isVisible = true;
    color = glm::vec4(1.0f, 0.0f, 0.2f, 1.0f); // R G B Alpha
    scale = 1.0f; // our global scale factor
    insertPt = new Point();
}

// translate vertices by the specified Point amount
void DrawObject::translateVertices(glm::vec3 point, std::vector<GLfloat> &vertices){
//    std::cout << glm::to_string(point) << std::endl;

    for(int i = 0; (i < vertices.size()); i=i+3) {
//        std::cout << i << " " << vertices.size() << std::endl;
        vertices[i]   = vertices[i]   + point.x; // x coord
        vertices[i+1] = vertices[i+1] + point.y; // y coord
        vertices[i+2] = vertices[i+2] + point.z; // y coord
    }
}

//scales the vertices of the object
void DrawObject::scaleObj(GLfloat scale_factor, std::vector<GLfloat> &vertices) {
    this->scale = scale_factor;
//    std::cout << " -- in DrawObj::scaleObj " << scale_factor << " " << vertices.size() << std::endl;
    for(int i = 0; i < vertices.size(); i++) {
//        std::cout << vertices[i];
        vertices[i] = vertices[i] * scale_factor;
//        std::cout << " " << vertices[i] << std::endl;
    }
}

// takes window coords and converts to World Coord using similar algorithm as 
// our ray cast conversion.
glm::vec3 DrawObject::convertMouseToWorldCoord(TestGLCanvas *canvas, glm::vec3 mouse_coord) {
    float x = (GLfloat)((2.0f * mouse_coord.x) / canvas->CANVAS_WIDTH - 1.0f);
    float y = (GLfloat)(1.0f - (2.0f * mouse_coord.y) / canvas->CANVAS_HEIGHT);
    float z = (GLfloat)(1.0f);
    glm::vec3 ray = glm::vec3 (x,y,z);

    glm::vec4 clip = glm::vec4(ray.x, ray.y, -1.0f, 1.0f);
    glm::vec4 eye = glm::inverse(canvas->getCamera()->projectionMatrix) * clip;
    eye = glm::vec4(eye.x, eye.y, -1.0f, 0.0f);

    glm::vec4 temp = (glm::inverse(canvas->getCamera()->viewMatrix) * eye);
//    std::cout << "- temp Vec4: " << glm::to_string(temp) << std::endl;
    std::cout << " -- temp: " << glm::to_string(temp) << std::endl;

    return glm::vec3(temp.x, temp.y, temp.z);
//    std::cout << "- ray_world: " << glm::to_string(temp) << std::endl;
}

// // copy constructor
// DrawObject::DrawObject(const DrawObject &source){
//     std::cout << "DrawObject copy constructor" << std::endl;

//     shader = source.shader;
//     isVisible = source.isVisible;
//     color = source.color;
// }

/////////////////////////////////////////////////////////////////////////
//  Crosshair drawing aid object
/////////////////////////////////////////////////////////////////////////

// default constructor
Crosshairs::Crosshairs(TestGLCanvas *canvas){
        std::cout << "Crosshair constructor" << std::endl;

//    setShader(new Shader(myvertexShaderSource, myfragmentShaderSource));
    setShader(new Shader());
    color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

void Crosshairs::Render(TestGLCanvas *orig_canvas)
{
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
    glUseProgram(getShader()->getShaderProgNum());
 //   glUseProgram(shaderProgram);
        
        // // Get the uniform locations
        GLint modelLoc = glGetUniformLocation(this->getShader()->getShaderProgNum(), "model");
        GLint viewLoc = glGetUniformLocation(this->getShader()->getShaderProgNum(), "view");
        GLint projLoc = glGetUniformLocation(this->getShader()->getShaderProgNum(), "projection");
    
        glm::mat4 identity = glm::mat4(); // so that the cross hairs arent scaled when they vertices are passed to the shaders

        GLint vertexColorLocation = glGetUniformLocation(this->getShader()->getShaderProgNum(), "vertexColor");


        // // Pass the matrices to the shader
        glUniform4f(vertexColorLocation, color.x, color.y, color.z, 1.0f);

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(identity));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(identity));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(identity));

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

void Crosshairs::CreateShaderProgram(){
    std::cout << "In Crosshairs Create Program: "  << std::endl;
}



Dice::Dice(TestGLCanvas *canvas){
    std::cout << "Dice constructor" << std::endl;
//    setShader(new Shader(myvertexShaderSource, myfragmentShaderSource));
    setShader(new Shader());
}


void Dice::Render(TestGLCanvas *orig_canvas) {
    // glClearColor(0.25f, 0.65f, 0.25f, 0.0);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -2.0f);
    GLfloat xangle = 0;
    GLfloat yangle = 0;
    glRotatef(xangle, 1.0f, 0.0f, 0.0f);
    glRotatef(yangle, 0.0f, 1.0f, 0.0f);

    // draw six faces of a cube of size 1 centered at (0, 0, 0)
 //   glBindTexture(GL_TEXTURE_2D, m_textures[0]);
    glBegin(GL_QUADS);
        glNormal3f( 0.0f, 0.0f, 1.0f);
        glTexCoord2f(0, 0); glVertex3f( 0.5f, 0.5f, 0.5f);
        glTexCoord2f(1, 0); glVertex3f(-0.5f, 0.5f, 0.5f);
        glTexCoord2f(1, 1); glVertex3f(-0.5f,-0.5f, 0.5f);
        glTexCoord2f(0, 1); glVertex3f( 0.5f,-0.5f, 0.5f);
    glEnd();

 //   glBindTexture(GL_TEXTURE_2D, m_textures[1]);
    glBegin(GL_QUADS);
        glNormal3f( 0.0f, 0.0f,-1.0f);
        glTexCoord2f(0, 0); glVertex3f(-0.5f,-0.5f,-0.5f);
        glTexCoord2f(1, 0); glVertex3f(-0.5f, 0.5f,-0.5f);
        glTexCoord2f(1, 1); glVertex3f( 0.5f, 0.5f,-0.5f);
        glTexCoord2f(0, 1); glVertex3f( 0.5f,-0.5f,-0.5f);
    glEnd();

 //   glBindTexture(GL_TEXTURE_2D, m_textures[2]);
    glBegin(GL_QUADS);
        glNormal3f( 0.0f, 1.0f, 0.0f);
        glTexCoord2f(0, 0); glVertex3f( 0.5f, 0.5f, 0.5f);
        glTexCoord2f(1, 0); glVertex3f( 0.5f, 0.5f,-0.5f);
        glTexCoord2f(1, 1); glVertex3f(-0.5f, 0.5f,-0.5f);
        glTexCoord2f(0, 1); glVertex3f(-0.5f, 0.5f, 0.5f);
    glEnd();

  //  glBindTexture(GL_TEXTURE_2D, m_textures[3]);
    glBegin(GL_QUADS);
        glNormal3f( 0.0f,-1.0f, 0.0f);
        glTexCoord2f(0, 0); glVertex3f(-0.5f,-0.5f,-0.5f);
        glTexCoord2f(1, 0); glVertex3f( 0.5f,-0.5f,-0.5f);
        glTexCoord2f(1, 1); glVertex3f( 0.5f,-0.5f, 0.5f);
        glTexCoord2f(0, 1); glVertex3f(-0.5f,-0.5f, 0.5f);
    glEnd();

  //  glBindTexture(GL_TEXTURE_2D, m_textures[4]);
    glBegin(GL_QUADS);
        glNormal3f( 1.0f, 0.0f, 0.0f);
        glTexCoord2f(0, 0); glVertex3f( 0.5f, 0.5f, 0.5f);
        glTexCoord2f(1, 0); glVertex3f( 0.5f,-0.5f, 0.5f);
        glTexCoord2f(1, 1); glVertex3f( 0.5f,-0.5f,-0.5f);
        glTexCoord2f(0, 1); glVertex3f( 0.5f, 0.5f,-0.5f);
    glEnd();

  //  glBindTexture(GL_TEXTURE_2D, m_textures[5]);
    glBegin(GL_QUADS);
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(0, 0); glVertex3f(-0.5f,-0.5f,-0.5f);
        glTexCoord2f(1, 0); glVertex3f(-0.5f,-0.5f, 0.5f);
        glTexCoord2f(1, 1); glVertex3f(-0.5f, 0.5f, 0.5f);
        glTexCoord2f(0, 1); glVertex3f(-0.5f, 0.5f,-0.5f);
    glEnd();
}


void Dice::AddVertex(){
    std::cout << "Add vertex for dice" << std::endl;
}

void Dice::CreateShaderProgram(){
    std::cout << "In dice Create Program: "  << std::endl;
}



////////////////////////////////////////////////////////////////////
// Our basic triangle shape
////////////////////////////////////////////////////////////////////
void Triangle::Render(TestGLCanvas *orig_canvas) {

    // Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f, // Left  
         0.5f, -0.5f, 0.0f, // Right 
         0.0f,  0.5f, 0.0f  // Top   
    };
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
//    glUseProgram(shaderProgram);
    glUseProgram(getShader()->getShaderProgNum());

        // // Get the uniform locations
        GLint modelLoc = glGetUniformLocation(this->getShader()->getShaderProgNum(), "model");
        GLint viewLoc = glGetUniformLocation(this->getShader()->getShaderProgNum(), "view");
        GLint projLoc = glGetUniformLocation(this->getShader()->getShaderProgNum(), "projection");
    
        // std::cout << "modelLoc: " << modelLoc << "viewLoc: " << viewLoc << "projLoc: " << projLoc << std::endl;

        GLint vertexColorLocation = glGetUniformLocation(this->getShader()->getShaderProgNum(), "vertexColor");

        // // Pass the matrices to the shader
        glUniform4f(vertexColorLocation, color.x, color.y, color.z, 1.0f);

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(orig_canvas->getCamera()->modelMatrix));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(orig_canvas->getCamera()->viewMatrix));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(orig_canvas->getCamera()->projectionMatrix));

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    // Properly de-allocate all resources once they've outlived their purpose
    glUseProgram(0); // stop using a shader program
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

}

Triangle::Triangle(TestGLCanvas *canvas){
    std::cout << "Triangle constructor" << std::endl;
//    setShader(new Shader(myvertexShaderSource, myfragmentShaderSource));
    setShader(new Shader());
}

void Triangle::AddVertex(){
    std::cout << "Add vertex for dice" << std::endl;
}

void Triangle::CreateShaderProgram(){
    std::cout << "In triangle Create Shader Program: "  << std::endl;
}

//////////////////////////////////////////////////////////////////////
//  Gridline drawing aid routines
//////////////////////////////////////////////////////////////////////
Gridlines::Gridlines(TestGLCanvas *canvas, GLfloat x, GLfloat y, GLfloat z, Gridline_Dir dir){
    std::cout << "Gridlines constructor" << std::endl;

    // x_spa = 0.10f;
    // y_spa = 0.20f;
    // z_spa = 0.10f;
    numSpaces1 = 20;
    numSpaces2 = 20;
    GLfloat spa1 = 0.25f;
    GLfloat spa2 = 0.25f;
    GLfloat lim1 = spa1 * numSpaces1;
    GLfloat lim2 = spa2 * numSpaces2;

    direction = dir;

    if(direction == OVERLAY) {
        std::cout << "Making default overlay" << std::endl;
        x_spa = x;
        y_spa = y;
        z_spa = z; 
        setShader(new Shader("shaders/test.vert","shaders/test.frag")); 
        makeGridDefaultOverlay(this->vertices);
        color = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
        isVisible = false;

    } else if(direction == XY_PLANE) {
        std::cout << "XY_PLANE" << std::endl;

        x_spa = x;
        y_spa = y;
        z_spa = 0.0f;
        setShader(new Shader("shaders/drawgrid.vert","shaders/drawgrid.frag")); 
        makeGridMesh(lim1, lim2, spa1, spa2, direction, this->vertices);
        isVisible = false;
        color = glm::vec4(0.5f, 0.3f, 0.3f, 0.25f);

        //        isVisible = true;
        //displayVertices();

    } else if(direction == XZ_PLANE) {
        std::cout << "XZ_PLANE" << std::endl;
        x_spa = x;
        y_spa = 0.0f;
        z_spa = z;
        setShader(new Shader("shaders/drawgrid.vert","shaders/drawgrid.frag")); 
        makeGridMesh(lim1, lim2, spa1, spa2, direction, this->vertices);
        isVisible = false;
        color = glm::vec4(0.3f, 0.5f, 0.3f, 0.25f);
        //isVisible = true;

    } else if(direction == YZ_PLANE) {
        std::cout << "YZ_PLANE" << std::endl;
        x_spa = 0.0f;
        y_spa = y;
        z_spa = z;
        setShader(new Shader("shaders/drawgrid.vert","shaders/drawgrid.frag")); 
        makeGridMesh(lim1, lim2, spa1, spa2, direction, this->vertices);
        isVisible = false;
        color = glm::vec4(0.3f, 0.3f, 0.5f, 0.25f);
        //isVisible = true;
    } else {
        std::cout << "Attempting to make invalid grid" << std::endl;
        return;
    }
}

// copy constructor
Gridlines::Gridlines(const Gridlines &source){
    std::cout << "Gridlines copy constructor" << std::endl;
    for(int i=0; i<source.vertices.size(); i++) {
        vertices.push_back(source.vertices[i]);
    }

    numSpaces1 = source.numSpaces1;
    numSpaces2 = source.numSpaces2;

    color = source.color;
    x_spa = source.x_spa;
    y_spa = source.y_spa;
    z_spa = source.z_spa;
    direction = source.direction;
}

void Gridlines::Render(TestGLCanvas *orig_canvas)
{
    int num_verts = vertices.size() / 3;
    // std::cout << "Vertices.size(): " << vertices.size() << std::endl;
    // std::cout << "Number of vertices: " << num_verts << std::endl;
    // displayVertices();
    
    // std::cout << curr_x << " , " << curr_y << std::endl;
    // std::cout << num_verts << std::endl;

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertices.size() / num_verts, (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

    // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
    glfwPollEvents();

    // Render

    // Draw our first triangle
    glUseProgram(getShader()->getShaderProgNum());

            // // Get the uniform locations
        GLint modelLoc = glGetUniformLocation(this->getShader()->getShaderProgNum(), "model");
        GLint viewLoc = glGetUniformLocation(this->getShader()->getShaderProgNum(), "view");
        GLint projLoc = glGetUniformLocation(this->getShader()->getShaderProgNum(), "projection");
    
        // std::cout << "modelLoc: " << modelLoc << "viewLoc: " << viewLoc << "projLoc: " << projLoc << std::endl;

        GLint vertexColorLocation = glGetUniformLocation(this->getShader()->getShaderProgNum(), "vertexColor");

        // // Pass the matrices to the shader
     glUniform4f(vertexColorLocation, color.x, color.y, color.z, 1.0f);

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(orig_canvas->getCamera()->modelMatrix));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(orig_canvas->getCamera()->viewMatrix));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(orig_canvas->getCamera()->projectionMatrix));

//    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, num_verts);
 //   glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);

    // Properly de-allocate all resources once they've outlived their purpose
    glUseProgram(0); // stop using a shader program
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

// utility function to display the coordinate date of the "vertices" vector in each drawing object.
void Gridlines::displayVertices(){
    int i = 1;
    for (const auto &coord : vertices) {
        std::cout << coord;

        if(i==3) {
            std::cout << std::endl;
            i=1;
        } else {
            std::cout << " , ";
            i++;
        }
    }
}

void Gridlines::makeGridMesh(GLfloat lim1, GLfloat lim2, GLfloat spa1, 
    GLfloat spa2, Gridline_Dir direction, std::vector<GLfloat> &vertices) {

//    std::cout << lim1 << " " << lim2 << " " << spa1 << " " << spa2 << " " << direction << std::endl;
//    std::cout << this->numSpaces1 <<  " " << this->numSpaces2 << std::endl;

    if (direction == XY_PLANE) {
        GLfloat x_val = lim1;
        GLfloat y_val = lim2;
        GLfloat z_val = 0.0f;

        vertices.push_back(-lim1);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);

        vertices.push_back(lim1);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);

        for(int i = 1; i < numSpaces2; i++) {
            vertices.push_back(-lim1);
            vertices.push_back(0.0f+i*spa1);
            vertices.push_back(0.0f);

            vertices.push_back(lim1);
            vertices.push_back(0.0f+i*spa1);
            vertices.push_back(0.0f);

            vertices.push_back(-lim1);
            vertices.push_back(0.0f-i*spa1);
            vertices.push_back(0.0f);

            vertices.push_back(lim1);
            vertices.push_back(0.0f-i*spa1);
            vertices.push_back(0.0f);
        }

        vertices.push_back(0.0f);
        vertices.push_back(-lim2);
        vertices.push_back(0.0f);

        vertices.push_back(0.0f);
        vertices.push_back(lim2);
        vertices.push_back(0.0f);

        for(int i = 1; i < numSpaces1; i++) {
            vertices.push_back(0.0f+i*spa2);
            vertices.push_back(-lim2);
            vertices.push_back(0.0f);

            vertices.push_back(0.0f+i*spa2);
            vertices.push_back(lim2);
            vertices.push_back(0.0f);

            vertices.push_back(0.0f-i*spa2);
            vertices.push_back(-lim2);
            vertices.push_back(0.0f);

            vertices.push_back(0.0f-i*spa2);
            vertices.push_back(lim2);
            vertices.push_back(0.0f);
        }

    } else if (direction == YZ_PLANE) {
        vertices.push_back(0.0f);
        vertices.push_back(-lim1);
        vertices.push_back(0.0f);

        vertices.push_back(0.0f);
        vertices.push_back(lim1);
        vertices.push_back(0.0f);

        for(int i = 1; i < numSpaces2; i++) {
            vertices.push_back(0.0f);
            vertices.push_back(-lim1);
            vertices.push_back(0.0f+i*spa1);

            vertices.push_back(0.0f);
            vertices.push_back(lim1);
            vertices.push_back(0.0f+i*spa1);

            vertices.push_back(0.0f);
            vertices.push_back(-lim1);
            vertices.push_back(0.0f-i*spa1);

            vertices.push_back(0.0f);
            vertices.push_back(lim1);
            vertices.push_back(0.0f-i*spa1);
        }

        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(-lim2);

        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(lim2);

        for(int i = 1; i < numSpaces1; i++) {
            vertices.push_back(0.0f);
            vertices.push_back(0.0f+i*spa2);
            vertices.push_back(-lim2);

            vertices.push_back(0.0f);
            vertices.push_back(0.0f+i*spa2);
            vertices.push_back(lim2);

            vertices.push_back(0.0f);
            vertices.push_back(0.0f-i*spa2);
            vertices.push_back(-lim2);

            vertices.push_back(0.0f);
            vertices.push_back(0.0f-i*spa2);
            vertices.push_back(lim2);
        }

    } else if (direction == XZ_PLANE) {
        vertices.push_back(-lim1);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);

        vertices.push_back(lim1);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);

        for(int i = 1; i < numSpaces2; i++) {
            vertices.push_back(-lim1);
            vertices.push_back(0.0f);
            vertices.push_back(0.0f+i*spa1);

            vertices.push_back(lim1);
            vertices.push_back(0.0f);
            vertices.push_back(0.0f+i*spa1);

            vertices.push_back(-lim1);
            vertices.push_back(0.0f);
            vertices.push_back(0.0f-i*spa1);

            vertices.push_back(lim1);
            vertices.push_back(0.0f);
            vertices.push_back(0.0f-i*spa1);
        }

        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(-lim2);

        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(lim2);

        for(int i = 1; i < numSpaces1; i++) {
            vertices.push_back(0.0f+i*spa2);
            vertices.push_back(0.0f);
            vertices.push_back(-lim2);

            vertices.push_back(0.0f+i*spa2);
            vertices.push_back(0.0f);
            vertices.push_back(lim2);

            vertices.push_back(0.0f-i*spa2);
            vertices.push_back(0.0f);
            vertices.push_back(-lim2);

            vertices.push_back(0.0f-i*spa2);
            vertices.push_back(0.0f);
            vertices.push_back(lim2);
        }
    } else {
        std::cout << "ERROR: making grid..." << std::endl;
    }
}

void Gridlines::makeGridDefaultOverlay(std::vector<GLfloat> &vertices) {
//    std::vector<GLfloat> vertices;
    // for centermost gridlines
    vertices.push_back(-1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);

    vertices.push_back(0.0f);
    vertices.push_back(-1.0f);
    vertices.push_back(0.0f);

    // for all other horiz lines
    for(int i = 1; i < 1.0 / y_spa; i++) {
        GLfloat top = 0.0f + (i * y_spa);

        // horiz line
        vertices.push_back(-1.0f);
        vertices.push_back(top);
        vertices.push_back(0.0f);

        vertices.push_back(1.0f);
        vertices.push_back(top);
        vertices.push_back(0.0f);

        vertices.push_back(-1.0f);
        vertices.push_back(-top);
        vertices.push_back(0.0f);

        vertices.push_back(1.0f);
        vertices.push_back(-top);
        vertices.push_back(0.0f);
    }

    // for all other vert lines
    for(int i = 1; i < 1.0 / x_spa; i++) {

        GLfloat left = 0.0f + (i * x_spa);

        // vert line
        vertices.push_back(left);
        vertices.push_back(1.0f);
        vertices.push_back(0.0f);

        vertices.push_back(left);
        vertices.push_back(-1.0f);
        vertices.push_back(0.0f);

        vertices.push_back(-left);
        vertices.push_back(1.0f);
        vertices.push_back(0.0f);

        vertices.push_back(-left);
        vertices.push_back(-1.0f);
        vertices.push_back(0.0f);

    }
}

void Gridlines::AddVertex(){
    std::cout << "Add vertex for gridlines" << std::endl;
}

void Gridlines::CreateShaderProgram(){
    std::cout << "In gridlines Create Shader Program: "  << std::endl;
}


////////////////////////////////////////////////////////
//  Our cursor object
////////////////////////////////////////////////////////

//constructor
CursorObj::CursorObj(DrawObject* object) {
    std::cout << "Create our cursor object" << std::endl;
    obj = object;
    lastPt = glm::vec3(0.0f, 0.0f, 0.0f);
    ray_nds = glm::vec3(0.0f, 0.0f, 0.0f);
    ray_clip = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    ray_eye = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    ray_world = glm::vec3(0.0f, 0.0f, 0.0f);
}

void CursorObj::moveCursor(TestGLCanvas *canvas, Point curr_pt) {
    // std::cout << "-- Moving cursor" << std::endl;

    // perform our ray casting to find the point
    this->normDeviceCoords(canvas, this->ray_nds);
    this->homogenClipCoords(this->ray_nds, this->ray_clip);
    this->eyeCameraCoords(canvas, ray_eye);
    this->worldCoords(canvas, ray_world);
    //displayRayCastInfo();

    // move the cursor object vertices by the displaced amount
    this->obj->translateVertices((intersectPt-lastPt), this->obj->vertices);
    lastPt = intersectPt; // store the intersect point as our last point
}

void CursorObj::Render(TestGLCanvas *orig_canvas) {
    obj->Render(orig_canvas);
    //std::cout << "drawing cursor" << std::endl;
}

void CursorObj::AddVertex(){
    std::cout << "Add vertex for cursor object" << std::endl;
}

void CursorObj::CreateShaderProgram(){
    std::cout << "In cursor object Create Shader Program: "  << std::endl;
}

// returns 3D Normalized Device coordinates (-1.0:1.0, -1.0:1.0, -1.0:1.0)
// based on a cursor's current 2D Viewport coords (0:width, height:0)
void CursorObj::normDeviceCoords(TestGLCanvas *canvas, glm::vec3 &ray_nds) {
    float x = (GLfloat)((2.0f * canvas->getCurrMousePos()->getX()) / canvas->CANVAS_WIDTH - 1.0f);
    float y = (GLfloat)(1.0f - (2.0f * canvas->getCurrMousePos()->getY()) / canvas->CANVAS_HEIGHT);
    float z = (GLfloat)(1.0f);
    ray_nds = glm::vec3 (x,y,z);
    return;
}

// returns 4D Homogeneous Clip Coords (-1:1, -1:1, -1:1, -1:1)
void CursorObj::homogenClipCoords(glm::vec3 ray, glm::vec4 &ray_clip){
    ray_clip = glm::vec4(ray.x, ray.y, -1.0f, 1.0f);
    return;
}

// returns 4D Eye (Camera) Coords in (-x:x, -y:y, -z:z, -w:w)
void CursorObj::eyeCameraCoords(TestGLCanvas *canvas, glm::vec4 &ray_eye) {
//    std::cout << "Projection Matrix: " << glm::to_string(canvas->getCamera()->projectionMatrix) << std::endl;
    ray_eye = glm::inverse(canvas->getCamera()->projectionMatrix) * ray_clip;
    ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0f, 0.0f);
    return;
} 

// returns 4D World Coords in (-x:x, -y:y, -z:z, -w:w)
void CursorObj::worldCoords(TestGLCanvas *canvas, glm::vec3 &ray_world) {
//    std::cout << "View Matrix: " << glm::to_string(canvas->getCamera()->viewMatrix) << std::endl;
    glm::vec4 temp = (glm::inverse(canvas->getCamera()->viewMatrix) * ray_eye);
//    std::cout << "- temp Vec4: " << glm::to_string(temp) << std::endl;

    ray_world = glm::vec3(temp.x, temp.y, temp.z);
//    std::cout << "- ray_world: " << glm::to_string(temp) << std::endl;

    ray_world = glm::normalize(ray_world);
//    std::cout << "- normalized ray_world Vec4: " << glm::to_string(ray_world) << std::endl;

    // assuming xy plane intersection
    GLfloat x = 0.0f;
    GLfloat y = 0.0f;
    GLfloat z = 0.0f;

    glm::vec3 cameraPt = glm::vec3(canvas->getCamera()->getPos());

    // for XY Plane intersection
    GLfloat t = (z - canvas->getCamera()->getPos().z / ray_world.z);
    // for XZ Plane intersection
    //    GLfloat t = (y - canvas->getCamera()->getPos().y / ray_world.y);
    // for YZ Plane intersection
    //    GLfloat t = (x - canvas->getCamera()->getPos().x / ray_world.x);
    // std::cout << "scale factor: " << t << std::endl;

    intersectPt = cameraPt + ray_world * t;
    // std::cout << "intersectPt: " << intersectPt.x << " , " << intersectPt.y << " , " << intersectPt.z << std::endl;

    return;
} 

void CursorObj::displayRayCastInfo(){
    std::cout << "===============================================================" << std::endl;
    std::cout << "normDeviceCoords: " << ray_nds.x << " , " << ray_nds.y << " , " << ray_nds.z << std::endl;
    std::cout << "rayClipCoords: " << ray_clip.x << " , " << ray_clip.y << " , " << ray_clip.z << " , " << ray_clip.w << std::endl;
    std::cout << "eyeCameraCoords: " << ray_eye.x << " , " << ray_eye.y << " , " << ray_eye.z << " , " << ray_eye.w << std::endl;
    std::cout << "rayCoords: " << ray_world.x << " , " << ray_world.y << " , " << ray_world.z << std::endl;
    std::cout << "===============================================================" << std::endl;
}
