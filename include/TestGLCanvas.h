/////////////////////////////////////////////////////////////////////////////
// Name:        TestGLCanvas.h
/////////////////////////////////////////////////////////////////////////////



#ifndef _TESTGLCANVAS_H_
#define _TESTGLCANVAS_H_

#include <vector>
#include "wx/glcanvas.h"

// Include GLM
#include </usr/include/glm/glm.hpp>
//#include </usr/include/glm/gtc/matrix_transform.hpp>

// forward declaration
//class wxGLCanvas;
class MyFrame;
class Point;
class DrawObject;
class TestGLContext;
class Camera;

class TestGLCanvas : public wxGLCanvas
{
public:
    static const int CANVAS_WIDTH = 600;
    static const int CANVAS_HEIGHT = 600;

    TestGLCanvas(wxWindow *parent, int *attribList = NULL);
    ~TestGLCanvas(); // destructor

    // methods
    GLuint getCurrentDrawNum() {return m_current_drawnum;}
    void setCurrentDrawNum(GLuint num) {m_current_drawnum = num;}
    void setCurrMousePos(int x, int y, int z);
    void setCurrMousePos(Point* point);

    Point* getCurrMousePos() {return m_currMousePos;}
    Camera* getCamera() {return m_camera;}

    // Returns the shared context used by all frames and sets it as current for
    // the given canvas.
    TestGLContext& GetContext(wxGLCanvas *canvas);

    void CreateDrawObj();  // creates our drafting aids (crosshairs, grid, etc.)
    void updateInfoBar();  // updates the information bar at bottom of window.
    void processCameraEvent(wxKeyEvent& event); // processes camera events

    DrawObject* m_xy_grid;
    DrawObject* m_yz_grid;
    DrawObject* m_xz_grid;
    
    DrawObject* m_cursor;
    DrawObject* m_gridlines;
    DrawObject* m_crosshair;
    TestGLContext* m_glContext;
    std::vector<DrawObject*> drawObjects;

private:
    Camera* m_camera;
    GLuint m_current_drawnum;
    Point* m_currMousePos;
    MyFrame *myParentFrame;
    //glm::vec3 *ray_nds;
    
    void OnPaint(wxPaintEvent& event);
    void Spin(float xSpin, float ySpin);
    void OnKeyDown(wxKeyEvent& event);
    void OnSpinTimer(wxTimerEvent& WXUNUSED(event));
    void OnMouseMove(wxMouseEvent& event);
    void OnMouseWheel(wxMouseEvent& event);

    void OnKeyPress(wxKeyEvent& event);

    // angles of rotation around x- and y- axis
    float m_xangle,
          m_yangle;

    wxTimer m_spinTimer;
    bool m_useStereo,
         m_stereoWarningAlreadyDisplayed;

    wxDECLARE_EVENT_TABLE();
};

#endif // _WX_TESTGLCANVAS_H_
