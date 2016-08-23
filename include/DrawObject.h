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

    void setShader(Shader *s) {shader = s; return;}
    Shader *getShader() {return shader;}

    bool isVisible;

private:
    Shader* shader;

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
    Gridlines(TestGLCanvas *canvas, GLfloat x = 0.5f, GLfloat y = 0.5f, GLfloat z=0.0f, Gridline_Dir dir = OVERLAY);

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
private:
    std::vector<GLfloat> vertices;
    GLfloat x_spa;
    GLfloat y_spa;
    GLfloat z_spa;
};

#endif // _DRAWOBJECT_H
