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
//#include "../include/Geometry.h"
//#include "../include/TestGLCanvas.h"

//forward declaration
class TestGLCanvas;
class TestGLContext;
//class DrawMenuDialog

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

enum
{
    NEW_STEREO_WINDOW = wxID_HIGHEST + 1,
    DRAW_ONE = wxID_HIGHEST + 2,
    DRAW_TWO = wxID_HIGHEST + 3,
    DRAW_SELECT = wxID_HIGHEST + 4,
    SpinTimer = wxID_HIGHEST + 5
};

// IMPLEMENT_APP(MyApp)
DECLARE_APP(MyApp)

#endif // _WX_CUBE_H_
