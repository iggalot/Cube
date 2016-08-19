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

// Include GLM
#include </usr/include/glm/glm.hpp>
#include </usr/include/glm/gtc/matrix_transform.hpp>
//#include </usr/include/glm/gtc/type_ptr.hpp>

#include "../include/cube.h"
#include "../include/Geometry.h"
#include "../include/TestGLCanvas.h"
#include "../include/TestGLContext.h"
#include "../include/DrawObject.h"
#include "../include/Shader.h"
#include "../include/Camera.h"

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
      m_spinTimer(this,SpinTimer)
 //     m_useStereo(false),
//      m_stereoWarningAlreadyDisplayed(false)
{
    m_glContext = NULL;
    m_current_drawnum = 0;
    m_currMousePos = new Point();
    m_crosshair = NULL;
    m_gridlines = NULL;
    m_camera = NULL;

//    m_crosshair = new Crosshairs(this);

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

    // this should get the frame that is the parent of the canvas.  Must cast it
    // to (MyFrame*) because the default GetParent() returns a wxWindow which
    // MyFrame is also derived from.
    myParentFrame = (MyFrame *) this->GetParent(); 


    glewExperimental = GL_TRUE;
    GLenum err = glewInit();

    if(err != GLEW_OK) {
        std::cout << "Error #: " << glGetError() << " str: " << glewGetErrorString(err) << std::endl;;

        std::cout << "GLEW_OK: " << GLEW_OK << std::endl;

        std::cout << "GL_NO_ERROR: " << GL_NO_ERROR << std::endl;
        std::cout << "GL_INVALID_ENUM: " << GL_INVALID_ENUM << std::endl;
        std::cout << "GL_INVALID_VALUE: " << GL_INVALID_VALUE << std::endl;
        std::cout << "GL_OUT_OF_MEMORY: " << GL_OUT_OF_MEMORY << std::endl;
        std::cout << "GL_STACK_UNDERFLOW: " << GL_STACK_UNDERFLOW << std::endl;
        std::cout << "GL_STACK_OVERFLOW: " << GL_STACK_OVERFLOW << std::endl;       
    } else
        std::cout << "GlewInit() okay" << std::endl; 

    // setup our camera object -- must be after glewInit();
    m_camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));

}

TestGLCanvas::~TestGLCanvas() {
    std::cout<< "In TestGLCanvas destructor" << std::endl;
    delete m_gridlines;
    delete m_crosshair;
    delete m_glContext;
    delete m_currMousePos;
    delete m_camera;
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
 //   std::cout << "1. In TestGLCanvas OnPaint:" << std::endl;

//    TestGLContext& curr_context = wxGetApp().mainFrame->GetContext(this);
//    TestGLContext& curr_context = GetContext(this);
//    std::cout << "2. In TestGLCanvas OnPaint:" << std::endl;

    glViewport(0, 0, ClientSize.x, ClientSize.y);

    // Render the graphics and swap the buffers.
    // GLboolean quadStereoSupported;
    // glGetBooleanv( GL_STEREO, &quadStereoSupported);
    // if ( quadStereoSupported )
    // {
    //     // glDrawBuffer( GL_BACK_LEFT );
    //     // glMatrixMode(GL_PROJECTION);
    //     // glLoadIdentity();
    //     // glFrustum(-0.47f, 0.53f, -0.5f, 0.5f, 1.0f, 3.0f);
    //     // canvas.DrawRotatedCube(m_xangle, m_yangle);
    //     // CheckGLError();
    //     // glDrawBuffer( GL_BACK_RIGHT );
    //     // glMatrixMode(GL_PROJECTION);
    //     // glLoadIdentity();
    //     // glFrustum(-0.53f, 0.47f, -0.5f, 0.5f, 1.0f, 3.0f);
    //     // canvas.DrawRotatedCube(m_xangle, m_yangle);
    //     // CheckGLError();
    // }
    // else
    // {
        // if(m_crosshair == NULL) {
        //     GetContext(this);
        //     m_crosshair = new Crosshairs(this);
        // }

//        curr_context.DrawRotatedCube(m_xangle, m_yangle);
    glClearColor(0.5f, 0.5f, 0.65f, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // // Create camera transformation
//    glm::mat4 model;
    // model = glm::mat4();  // for an identity matrix

    // // this will make the object progressively get further away
    // getCamera()->modelMatrix = glm::translate(getCamera()->modelMatrix, glm::vec3(0.0f, 0.0f, -0.5f));
 
    // this will make the object remain in its current position
    getCamera()->modelMatrix = glm::translate(getCamera()->modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));


//    glm::mat4 view;
    getCamera()->viewMatrix = getCamera()->GetViewMatrix();
//    glm::mat4 projection;
    getCamera()->projectionMatrix = glm::perspective(getCamera()->Zoom, (float)CANVAS_WIDTH/(float)CANVAS_HEIGHT, 0.1f, 1000.0f);



    for(auto &drawobj : this->drawObjects ) {
        drawobj->Render(this);
    }
        //This line will reinstall the spinning cube and the basic triangle, in lieu of
        // the more general drawing object inheritance object
//        GetContext(this).DrawRotatedCube(m_xangle, m_yangle);


        // if ( m_useStereo && !m_stereoWarningAlreadyDisplayed )
        // {
        //     m_stereoWarningAlreadyDisplayed = true;
        //     wxLogError("Stereo not supported by the graphics card.");
        // }
    glFlush();

        //CheckGLError();
//    }
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
    //std::cout << "m_glContext: " << m_glContext << std::endl;
    if ( !m_glContext )
    {
        // Create the OpenGL context for the first mono window which needs it:
        // subsequently created windows will all share the same context.
        m_glContext = new TestGLContext(canvas);
    }

//    std::cout << "1. TestGLCanvas::GetContext" << std::endl;
    m_glContext->SetCurrent(*canvas);
//   std::cout << "2. TestGLCanvas::GetContext" << std::endl;
    return *m_glContext;
}

void TestGLCanvas::CreateDrawObj() {

    std::cout << "In TestGLCanvas CreateDrawObj" << std::endl;
    GetContext(this);

     //   DrawObject *dice = new Dice(this);
    // std::cout << "-- Create Dice" << std::endl;
    // drawObjects.push_back(new Dice(this));
    std::cout << "-- Create Triangle" << std::endl;
    drawObjects.push_back(new Triangle(this));

    std::cout << "-- Create Crosshairs" << std::endl;
    // create our crosshairs
    if(m_crosshair == NULL) {
        m_crosshair = new Crosshairs(this);
        drawObjects.push_back(m_crosshair);
    }

    std::cout << "-- Create Gridlines" << std::endl;
    // create our gridlines
    if(m_gridlines == NULL) {
//        m_gridlines = new Gridlines(this);
        m_gridlines = new Gridlines(this, 0.25f, 0.10f, 0.10f);
        drawObjects.push_back(m_gridlines);
    }

    // Add a drawing item to our list to test (this is the dice from the TestGLContext)


}