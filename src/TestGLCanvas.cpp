///////////////////////////////////////////////////////////////////////////////
// Name:        cube.cpp
// Purpose:     wxGLCanvas demo program
// Author:      Julian Smart
// Modified by: Vadim Zeitlin to use new wxGLCanvas API (2007-04-09)
// Created:     04/01/98
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

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
#include "../include/TestGLContext.h"
#include "../include/DrawObject.h"

// ----------------------------------------------------------------------------
// TestGLCanvas
// ----------------------------------------------------------------------------

wxBEGIN_EVENT_TABLE(TestGLCanvas, wxGLCanvas)
    EVT_PAINT(TestGLCanvas::OnPaint)
    EVT_KEY_DOWN(TestGLCanvas::OnKeyDown)
    EVT_TIMER(SpinTimer, TestGLCanvas::OnSpinTimer)
    EVT_MOTION(TestGLCanvas::OnMouseMove)
wxEND_EVENT_TABLE()

TestGLCanvas::TestGLCanvas(wxWindow *parent, int *attribList)
    // With perspective OpenGL graphics, the wxFULL_REPAINT_ON_RESIZE style
    // flag should always be set, because even making the canvas smaller should
    // be followed by a paint event that updates the entire canvas with new
    // viewport settings.
    : wxGLCanvas(parent, wxID_ANY, attribList,
                 wxDefaultPosition, 
                 wxSize(CANVAS_WIDTH,CANVAS_HEIGHT),
//                 wxDefaultSize,
                 wxFULL_REPAINT_ON_RESIZE),
      m_xangle(30.0),
      m_yangle(30.0),
      m_spinTimer(this,SpinTimer),
      m_useStereo(false),
      m_stereoWarningAlreadyDisplayed(false)
{
    m_current_drawnum = 0;
    m_currMousePos = new Point();
    m_crosshair = new Crosshairs();

//    TestGLContext& curr_context = wxGetApp().mainFrame->GetContext(this);
//    GetContext(this);


    if ( attribList )
    {
        int i = 0;
        while ( attribList[i] != 0 )
        {
            if ( attribList[i] == WX_GL_STEREO )
                m_useStereo = true;
            ++i;
        }
    }
    // // EVT_PAINT(TestGLCanvas::OnPaint)
    // // EVT_KEY_DOWN(TestGLCanvas::OnKeyDown)
    // // EVT_TIMER(SpinTimer, TestGLCanvas::OnSpinTimer)

    // Connect(wxID_NEW, wxEVT_COMMAND_MENU_SELECTED,
    //    wxCommandEventHandler(MyFrame::OnNewWindow));

    // this should get the frame that is the parent of the canvas.  Must case it
    // to (MyFrame*) because the default GetParent() returns a wxWindow which
    // MyFrame is also derived from.
    myParentFrame = (MyFrame *) this->GetParent();  
}

void TestGLCanvas::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    //std::cout << "In TestGLCanvas OnPaint:" << std::endl;
    // This is required even though dc is not used otherwise.
    wxPaintDC dc(this);

    // Set the OpenGL viewport according to the client size of this canvas.
    // This is done here rather than in a wxSizeEvent handler because our
    // OpenGL rendering context (and thus viewport setting) is used with
    // multiple canvases: If we updated the viewport in the wxSizeEvent
    // handler, changing the size of one canvas causes a viewport setting that
    // is wrong when next another canvas is repainted.
    const wxSize ClientSize = GetClientSize();

 //   TestGLContext& canvas = wxGetApp().GetContext(this);
    std::cout << "1. In TestGLCanvas OnPaint:" << std::endl;

//    TestGLContext& curr_context = wxGetApp().mainFrame->GetContext(this);

//    TestGLContext& curr_context = GetContext(this);
    std::cout << "2. In TestGLCanvas OnPaint:" << std::endl;

    glViewport(0, 0, ClientSize.x, ClientSize.y);

    // Render the graphics and swap the buffers.
    GLboolean quadStereoSupported;
    glGetBooleanv( GL_STEREO, &quadStereoSupported);
    if ( quadStereoSupported )
    {
        // glDrawBuffer( GL_BACK_LEFT );
        // glMatrixMode(GL_PROJECTION);
        // glLoadIdentity();
        // glFrustum(-0.47f, 0.53f, -0.5f, 0.5f, 1.0f, 3.0f);
        // canvas.DrawRotatedCube(m_xangle, m_yangle);
        // CheckGLError();
        // glDrawBuffer( GL_BACK_RIGHT );
        // glMatrixMode(GL_PROJECTION);
        // glLoadIdentity();
        // glFrustum(-0.53f, 0.47f, -0.5f, 0.5f, 1.0f, 3.0f);
        // canvas.DrawRotatedCube(m_xangle, m_yangle);
        // CheckGLError();
    }
    else
    {
//        curr_context.DrawRotatedCube(m_xangle, m_yangle);
        GetContext(this).DrawRotatedCube(m_xangle, m_yangle);
        // if ( m_useStereo && !m_stereoWarningAlreadyDisplayed )
        // {
        //     m_stereoWarningAlreadyDisplayed = true;
        //     wxLogError("Stereo not supported by the graphics card.");
        // }
    }
    SwapBuffers();
}

void TestGLCanvas::Spin(float xSpin, float ySpin)
{
    m_xangle += xSpin;
    m_yangle += ySpin;

    Refresh(false);
}

void TestGLCanvas::OnKeyDown(wxKeyEvent& event)
{
    float angle = 5.0;

    switch ( event.GetKeyCode() )
    {
        case WXK_RIGHT:
            Spin( 0.0, -angle );
            break;

        case WXK_LEFT:
            Spin( 0.0, angle );
            break;

        case WXK_DOWN:
            Spin( -angle, 0.0 );
            break;

        case WXK_UP:
            Spin( angle, 0.0 );
            break;

        case WXK_SPACE:
            if ( m_spinTimer.IsRunning() )
                m_spinTimer.Stop();
            else
                m_spinTimer.Start( 25 );
            break;

        default:
            event.Skip();
            return;
    }
}

void TestGLCanvas::OnSpinTimer(wxTimerEvent& WXUNUSED(event))
{
    Spin(0.0, 4.0);
}

void TestGLCanvas::OnMouseMove(wxMouseEvent& event)
{
    setCurrMousePos(event.GetX(), event.GetY(), 0);

    this->myParentFrame->getStaticLabel()->SetLabel(
        wxT("Window Coords: ")
        +wxString::FromDouble(event.GetX())
        +wxT(",")
        +wxString::FromDouble(event.GetY())
    );

    Refresh();  // mark the frame as dirty
    //   Update();  // immediately update the window

    // std::cout << "Coord: (" << wxString::FromDouble(event.GetX()) << " , " << 
    //          wxString::FromDouble(event.GetY()) << ")" << std::endl;
    // // wxMessageBox("X Coordinate: "+wxString::FromDouble(event.GetX())+"\nY Coordinate: "+wxString::FromDouble(event.GetY()),
    // //    "Mouse Move Event",
    // //    wxOK);  
}

// wxString glGetwxString(GLenum name)
// {
//     const GLubyte *v = glGetString(name);
//     if ( v == 0 )
//     {
//         // The error is not important. It is GL_INVALID_ENUM.
//         // We just want to clear the error stack.
//         glGetError();

//         return wxString();
//     }

//     return wxString((const char*)v);
// }

void TestGLCanvas::setCurrMousePos(int x, int y, int z) {
    delete m_currMousePos; 
    m_currMousePos = new Point(x,y,z);
}

TestGLContext& TestGLCanvas::GetContext(wxGLCanvas *canvas)
{
    //TestGLContext *glContext;

    if ( !m_glContext )
    {
        // Create the OpenGL context for the first mono window which needs it:
        // subsequently created windows will all share the same context.
        m_glContext = new TestGLContext(canvas);
    }

    std::cout << "1. TestGLCanvas::GetContext" << std::endl;
    m_glContext->SetCurrent(*canvas);
    std::cout << "2. TestGLCanvas::GetContext" << std::endl;
    return *m_glContext;
}
