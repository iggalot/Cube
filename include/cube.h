/////////////////////////////////////////////////////////////////////////////
// Name:        cube.h
// Purpose:     wxGLCanvas demo program
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////



#ifndef _WX_CUBE_H_
#define _WX_CUBE_H_

#include "wx/glcanvas.h"

//forward declaration
class TestGLCanvas;
//class DrawMenuDialog

class Point 
{
public:
    Point();
    Point(int x, int y, int z);

    GLint x;
    GLint y;
    GLint z;    
};

// the rendering context used by all GL canvases
class TestGLContext : public wxGLContext
{
public:
    TestGLContext(wxGLCanvas *canvas);

    // render the cube showing it at given angles
    void DrawRotatedCube(float xangle, float yangle);
    void DrawCursor();
    void DoDrawCubeOne(float xangle, float yangle);
    void DoDrawCubeTwo(float xangle, float yangle);

private:
    // textures for the cube faces
    TestGLCanvas *orig_canvas;
    GLuint m_textures[6];
};

// Define a new application type
class MyApp : public wxApp
{
public:
    MyApp() { m_glContext = NULL; m_glStereoContext = NULL; }

    // Returns the shared context used by all frames and sets it as current for
    // the given canvas.
    TestGLContext& GetContext(wxGLCanvas *canvas);

    // virtual wxApp methods
    virtual bool OnInit();
    virtual int OnExit();

private:
    // the GL context we use for all our mono rendering windows
    TestGLContext *m_glContext;
    // the GL context we use for all our stereo rendering windows
    TestGLContext *m_glStereoContext;
};

// Define a new frame type
class MyFrame : public wxFrame
{
public:
    MyFrame(bool stereoWindow = false);
    wxStaticText* getStaticLabel() {return m_staticlabel;}
 
private:
    TestGLCanvas *m_canvas;
    wxStaticText *m_staticlabel;  // for displaying drawing screen information

    void OnClose(wxCommandEvent& event);
    void OnNewWindow(wxCommandEvent& event);
    void OnFile(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void OnSaveAs(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnDrawOne(wxCommandEvent& event);
    void OnDrawTwo(wxCommandEvent& event);
    void OnDrawSelect(wxCommandEvent& event);
};

// // Define the member selection frame
// class MemberSelectFrame : public wxFrame
// {
// public:
//     MemberSelectFrame();

// private:

// };

class TestGLCanvas : public wxGLCanvas
{
public:
    static const GLint CANVAS_HEIGHT = 600;
    static const GLint CANVAS_WIDTH = 600;

    TestGLCanvas(wxWindow *parent, int *attribList = NULL);
    GLuint getCurrentDrawNum() {return m_current_drawnum;}
    void setCurrentDrawNum(GLuint num) {m_current_drawnum = num;}
    Point *getCurrMousePos() {return currMousePos;}
    void setCurrMousePos(GLint x, GLint y, GLint z);  // records the mouse loc in window coords

    
private:
    Point *currMousePos;           // for storing the current mouse position
    GLuint m_current_drawnum;
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

enum
{
    NEW_STEREO_WINDOW = wxID_HIGHEST + 1,
    DRAW_ONE = wxID_HIGHEST + 2,
    DRAW_TWO = wxID_HIGHEST + 3,
    DRAW_SELECT = wxID_HIGHEST + 4,
    SpinTimer = wxID_HIGHEST + 5
};

#endif // _WX_CUBE_H_
