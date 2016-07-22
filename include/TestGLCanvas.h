/////////////////////////////////////////////////////////////////////////////
// Name:        TestGLCanvas.h
/////////////////////////////////////////////////////////////////////////////



#ifndef _TESTGLCANVAS_H_
#define _TESTGLCANVAS_H_

//#include "wx/glcanvas.h"
#include "wx/glcanvas.h"
//#include "../include/cube.h"
//#include "../include/Geometry.h"

// forward declaration
class MyFrame;
class Point;

class TestGLCanvas : public wxGLCanvas
{
public:
    static const int CANVAS_WIDTH = 600;
    static const int CANVAS_HEIGHT = 600;

    TestGLCanvas(wxWindow *parent, int *attribList = NULL);

    // methods
    GLuint getCurrentDrawNum() {return m_current_drawnum;}
    void setCurrentDrawNum(GLuint num) {m_current_drawnum = num;}
    void setCurrMousePos(int x, int y, int z) {
        delete m_currMousePos; 
        m_currMousePos = new Point(x,y,z);
    }
    Point* getCurrMousePos() {return m_currMousePos;}

private:
    GLuint m_current_drawnum;
    Point* m_currMousePos;
    MyFrame *myParentFrame;
    void OnPaint(wxPaintEvent& event);
    void Spin(float xSpin, float ySpin);
    void OnKeyDown(wxKeyEvent& event);
    void OnSpinTimer(wxTimerEvent& WXUNUSED(event));
    void OnMouseMove(wxMouseEvent& event);

    // angles of rotation around x- and y- axis
    float m_xangle,
          m_yangle;

    wxTimer m_spinTimer;
    bool m_useStereo,
         m_stereoWarningAlreadyDisplayed;

    wxDECLARE_EVENT_TABLE();
};

#endif // _WX_TESTGLCANVAS_H_
