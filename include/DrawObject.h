/////////////////////////////////////////////////////////////////////////////
// Name:        DrawObject.h
// Controls the assembly and render of various drawing objects used by the
// draw routines in the TestGLContext class
/////////////////////////////////////////////////////////////////////////////

#ifndef _DRAWOBJECT_H
#define _DRAWOBJECT_H

// forward declarations
class DrawObject;
class TestGLCanvas;

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

private:
    VertexData* vertices;
    VertexData* last;
    VertexData* head;

    GLuint* shaderProgram;
    GLuint* vao;
    GLuint* vbo;
    GLuint* ebo;
    GLuint* textures;
};

class Crosshairs : public DrawObject
{
public:
    Crosshairs(TestGLCanvas *canvas);

    void Render(TestGLCanvas* canvas);
    void AddVertex();
    void CreateShaderProgram();
};

class Dice : public DrawObject
{
public:
    Dice(TestGLCanvas *canvas);

    void Render(TestGLCanvas* canvas);
    void AddVertex();
    void CreateShaderProgram();
};

class Triangle : public DrawObject{
public:
    Triangle(TestGLCanvas *canvas);

    void Render(TestGLCanvas* canvas);
    void AddVertex();
    void CreateShaderProgram();
};

class Gridlines : public DrawObject{
public:
    //Gridlines(TestGLCanvas *canvas);
    Gridlines(TestGLCanvas *canvas, GLfloat x, GLfloat y, GLfloat z);

    // copy constructor
    Gridlines(const Gridlines &source);

    void Render(TestGLCanvas* canvas);
    void AddVertex();
    void CreateShaderProgram();

    void displayVertices();

private:
    std::vector<GLfloat> vertices;
    GLfloat x_spa;
    GLfloat y_spa;
    GLfloat z_spa;
};

#endif // _DRAWOBJECT_H
