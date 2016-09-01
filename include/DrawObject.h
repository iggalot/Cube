/////////////////////////////////////////////////////////////////////////////
// Name:        DrawObject.h
// Controls the assembly and render of various drawing objects used by the
// draw routines in the TestGLContext class
/////////////////////////////////////////////////////////////////////////////

#ifndef _DRAWOBJECT_H
#define _DRAWOBJECT_H

#include <vector>
#include </usr/include/glm/glm.hpp>

// forward declarations
class DrawObject;
class TestGLCanvas;
class Shader;

class VertexData {
public:
    VertexData();
    VertexData(GLfloat x_pt, GLfloat y_pt, GLfloat z_pt);

    VertexData *next;
    VertexData *prev;

    // methods

private:
    GLfloat x;
    GLfloat y;
    GLfloat z;
};

class DrawObject {
public:
    DrawObject();

    // methods
    virtual void AddVertex() = 0;
    virtual void Render(TestGLCanvas* canvas) = 0;
    virtual void CreateShaderProgram() = 0;

    void translateVertices(Point point, std::vector<GLfloat> &vertices);
    void scaleObj(GLfloat scale_factor, std::vector<GLfloat> &vertices);

    void setShader(Shader *s) {shader = s; return;}
    Shader *getShader() {return shader;}
    std::vector<GLfloat> getVertices() {return vertices;}
    Point *getInsertPt() {return insertPt;}

    GLfloat scale;
    bool isVisible;
    glm::vec4 color;
    std::vector<GLfloat> vertices;

private:

    Shader* shader;
    Point* insertPt;
    // VertexData* vertices;
    // VertexData* last;
    // VertexData* head;  
    // GLuint* vao;
    // GLuint* vbo;
    // GLuint* ebo;
    // GLuint* textures;
};

class Crosshairs : public DrawObject
{
public:
    Crosshairs(TestGLCanvas *canvas);

    void Render(TestGLCanvas* canvas);
    void AddVertex();
    void CreateShaderProgram();

//    std::vector<GLfloat> vertices;
};

class Dice : public DrawObject
{
public:
    Dice(TestGLCanvas *canvas);

    void Render(TestGLCanvas* canvas);
    void AddVertex();
    void CreateShaderProgram();

//    std::vector<GLfloat> vertices;
};

class Triangle : public DrawObject{
public:
    Triangle(TestGLCanvas *canvas);

    void Render(TestGLCanvas* canvas);
    void AddVertex();
    void CreateShaderProgram();

//    std::vector<GLfloat> vertices;
};

//////////////////////////////////////////////////////////////////////////
// Gridline class
//////////////////////////////////////////////////////////////////////////
enum Gridline_Dir {
    OVERLAY,
    XY_PLANE,
    YZ_PLANE,
    XZ_PLANE
};

class Gridlines : public DrawObject{
public:
    //Gridlines(TestGLCanvas *canvas);
    Gridlines(TestGLCanvas *canvas, GLfloat x = 0.5f, GLfloat y = 0.5f, GLfloat z=0.0f, 
        Gridline_Dir dir = OVERLAY);

    // copy constructor
    Gridlines(const Gridlines &source);

    void Render(TestGLCanvas* canvas);
    void AddVertex();
    void CreateShaderProgram();

    void displayVertices();

    void makeGridDefaultOverlay(std::vector<GLfloat> &vertices);
    void makeGridMesh(GLfloat lim1, GLfloat lim2, GLfloat spa1, 
            GLfloat spa2, Gridline_Dir direction, std::vector<GLfloat> &vertices);
    Gridline_Dir direction;



    int numSpaces1;  // number of spaces in direction1
    int numSpaces2;  // number of spaces in direction2

    GLfloat scale;

//    std::vector<GLfloat> vertices;

private:
    GLfloat x_spa;
    GLfloat y_spa;
    GLfloat z_spa;
};

class CursorObj : public DrawObject{
public:
    CursorObj(DrawObject* object);

    void Render(TestGLCanvas* canvas);
    void AddVertex();
    void CreateShaderProgram();

    void moveCursor(TestGLCanvas *canvas, Point* offset);

    // ray-casting methods
    void normDeviceCoords(TestGLCanvas *canvas, glm::vec3 &ray_coord); // transforms our mouse position into normalized device coords.
    void homogenClipCoords(glm::vec3 ray, glm::vec4 &ray_clip_coord);
    void eyeCameraCoords(TestGLCanvas *canvas, glm::vec4 &ray_eye);
    void worldCoords(TestGLCanvas *canvas, glm::vec3 &ray_world);
    void displayRayCastInfo();  // displays current ray values

    DrawObject *obj;
    Point* lastPt;  // the previous cursor point

 //   std::vector<GLfloat> vertices;

    // our ray cast routine variables
    glm::vec3 ray_nds;  // normalized device coords based on cursor position
    glm::vec4 ray_clip; // homogeneous clip coords
    glm::vec4 ray_eye;  // eye (camera) coordinates
    glm::vec3 ray_world; // world coordinates

private:

};

#endif // _DRAWOBJECT_H
