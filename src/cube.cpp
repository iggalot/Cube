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
#include "../include/DrawMenuDialog.h"

// #ifndef wxHAS_IMAGES_IN_RESOURCES
//     #include "../../sample.xpm"
// #endif

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// control ids
// enum
// {
//     SpinTimer = wxID_HIGHEST + 1
// };

// -----------------------------------------------
// Shader functions
// ------------------------------------------------
// Shaders -- remember the highp modifier on vec4 for 3.0 ES GLSL shaders
const GLchar* vertexShaderSource = "#version 300 es\n"
    "layout (location = 0) in vec3 position;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
    "}\0";
const GLchar* fragmentShaderSource = "#version 300 es\n"
    "out highp vec4 color;\n"
    "void main()\n"
    "{\n"
    "color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";


// ----------------------------------------------------------------------------
// helper functions
// ----------------------------------------------------------------------------

static void CheckGLError()
{
    GLenum errLast = GL_NO_ERROR;

    for ( ;; )
    {
        GLenum err = glGetError();
        if ( err == GL_NO_ERROR )
            return;

        // normally the error is reset by the call to glGetError() but if
        // glGetError() itself returns an error, we risk looping forever here
        // so check that we get a different error than the last time
        if ( err == errLast )
        {
            wxLogError(wxT("OpenGL error state couldn't be reset."));
            return;
        }

        errLast = err;

        wxLogError(wxT("OpenGL error %d"), err);
    }
}

// function to draw the texture for cube faces
static wxImage DrawDice(int size, unsigned num)
{
    wxASSERT_MSG( num >= 1 && num <= 6, wxT("invalid dice index") );

    const int dot = size/16;        // radius of a single dot
    const int gap = 5*size/32;      // gap between dots

    wxBitmap bmp(size, size);
    wxMemoryDC dc;
    dc.SelectObject(bmp);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();
    dc.SetBrush(*wxBLACK_BRUSH);

    // // the upper left and lower right points
    if ( num != 1 )
    {
        dc.DrawCircle(gap + dot, gap + dot, dot);
        dc.DrawCircle(size - gap - dot, size - gap - dot, dot);
    }

    // draw the central point for odd dices
    if ( num % 2 )
    {
        dc.DrawCircle(size/2, size/2, dot);
    }

    // the upper right and lower left points
    if ( num > 3 )
    {
        dc.DrawCircle(size - gap - dot, gap + dot, dot);
        dc.DrawCircle(gap + dot, size - gap - dot, dot);
    }

    // finally those 2 are only for the last dice
    if ( num == 6 )
    {
        dc.DrawCircle(gap + dot, size/2, dot);
        dc.DrawCircle(size - gap - dot, size/2, dot);
    }

    dc.SelectObject(wxNullBitmap);

    return bmp.ConvertToImage();
}

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// TestGLContext
// ----------------------------------------------------------------------------

TestGLContext::TestGLContext(wxGLCanvas *canvas)
             : wxGLContext(canvas)
{
    orig_canvas = (TestGLCanvas*) canvas;
    SetCurrent(*canvas);

    // set up the parameters we want to use
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_TEXTURE_2D);

    // add slightly more light, the default lighting is rather dark
    GLfloat ambient[] = { 0.5, 0.5, 0.5, 0.5 };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

    // set viewing projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-0.5f, 0.5f, -0.5f, 0.5f, 1.0f, 3.0f);

    // create the textures to use for cube sides: they will be reused by all
    // canvases (which is probably not critical in the case of simple textures
    // we use here but could be really important for a real application where
    // each texture could take many megabytes)
    glGenTextures(WXSIZEOF(m_textures), m_textures);

    for ( unsigned i = 0; i < WXSIZEOF(m_textures); i++ )
    {
        glBindTexture(GL_TEXTURE_2D, m_textures[i]);

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        const wxImage img(DrawDice(256, i + 1));

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.GetWidth(), img.GetHeight(),
                     0, GL_RGB, GL_UNSIGNED_BYTE, img.GetData());
    }

    CheckGLError();
}

void TestGLContext::DoDrawCubeOne(float xangle, float yangle) {
    glClearColor(0.25f, 0.65f, 0.25f, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -2.0f);
    glRotatef(xangle, 1.0f, 0.0f, 0.0f);
    glRotatef(yangle, 0.0f, 1.0f, 0.0f);

    // draw six faces of a cube of size 1 centered at (0, 0, 0)
    glBindTexture(GL_TEXTURE_2D, m_textures[0]);
    glBegin(GL_QUADS);
        glNormal3f( 0.0f, 0.0f, 1.0f);
        glTexCoord2f(0, 0); glVertex3f( 0.5f, 0.5f, 0.5f);
        glTexCoord2f(1, 0); glVertex3f(-0.5f, 0.5f, 0.5f);
        glTexCoord2f(1, 1); glVertex3f(-0.5f,-0.5f, 0.5f);
        glTexCoord2f(0, 1); glVertex3f( 0.5f,-0.5f, 0.5f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, m_textures[1]);
    glBegin(GL_QUADS);
        glNormal3f( 0.0f, 0.0f,-1.0f);
        glTexCoord2f(0, 0); glVertex3f(-0.5f,-0.5f,-0.5f);
        glTexCoord2f(1, 0); glVertex3f(-0.5f, 0.5f,-0.5f);
        glTexCoord2f(1, 1); glVertex3f( 0.5f, 0.5f,-0.5f);
        glTexCoord2f(0, 1); glVertex3f( 0.5f,-0.5f,-0.5f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, m_textures[2]);
    glBegin(GL_QUADS);
        glNormal3f( 0.0f, 1.0f, 0.0f);
        glTexCoord2f(0, 0); glVertex3f( 0.5f, 0.5f, 0.5f);
        glTexCoord2f(1, 0); glVertex3f( 0.5f, 0.5f,-0.5f);
        glTexCoord2f(1, 1); glVertex3f(-0.5f, 0.5f,-0.5f);
        glTexCoord2f(0, 1); glVertex3f(-0.5f, 0.5f, 0.5f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, m_textures[3]);
    glBegin(GL_QUADS);
        glNormal3f( 0.0f,-1.0f, 0.0f);
        glTexCoord2f(0, 0); glVertex3f(-0.5f,-0.5f,-0.5f);
        glTexCoord2f(1, 0); glVertex3f( 0.5f,-0.5f,-0.5f);
        glTexCoord2f(1, 1); glVertex3f( 0.5f,-0.5f, 0.5f);
        glTexCoord2f(0, 1); glVertex3f(-0.5f,-0.5f, 0.5f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, m_textures[4]);
    glBegin(GL_QUADS);
        glNormal3f( 1.0f, 0.0f, 0.0f);
        glTexCoord2f(0, 0); glVertex3f( 0.5f, 0.5f, 0.5f);
        glTexCoord2f(1, 0); glVertex3f( 0.5f,-0.5f, 0.5f);
        glTexCoord2f(1, 1); glVertex3f( 0.5f,-0.5f,-0.5f);
        glTexCoord2f(0, 1); glVertex3f( 0.5f, 0.5f,-0.5f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, m_textures[5]);
    glBegin(GL_QUADS);
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(0, 0); glVertex3f(-0.5f,-0.5f,-0.5f);
        glTexCoord2f(1, 0); glVertex3f(-0.5f,-0.5f, 0.5f);
        glTexCoord2f(1, 1); glVertex3f(-0.5f, 0.5f, 0.5f);
        glTexCoord2f(0, 1); glVertex3f(-0.5f, 0.5f,-0.5f);
    glEnd();
}

void TestGLContext::DoDrawCubeTwo(float xangle, float yangle) {
    glClearColor(0.65f, 0.25f, 0.25f, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();
std::cout << "Drawing cube 2" << std::endl;

// Build and compile our shader program
    // Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Check for compile time errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // Check for compile time errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Link shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

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
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    // Properly de-allocate all resources once they've outlived their purpose
    glUseProgram(0); // stop using a shader program
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

}

void TestGLContext::DrawCrosshairs()
{
    glewInit();
    std::cout << "Drawing cursor" << std::endl;

// Build and compile our shader program
    // Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Check for compile time errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // Check for compile time errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Link shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

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

    // Draw our first triangle
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, num_verts);
    glBindVertexArray(0);

    // Properly de-allocate all resources once they've outlived their purpose
    glUseProgram(0); // stop using a shader program
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);





}


void TestGLContext::DrawRotatedCube(float xangle, float yangle)
{
    // glClearColor(0.65f, 0.25f, 0.25f, 0.0);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    DoDrawCubeOne(xangle, yangle);

    if(orig_canvas->getCurrentDrawNum() == 0) {
        DoDrawCubeOne(xangle, yangle);
    } else {
        DoDrawCubeTwo(xangle, yangle);
    }

    DrawCrosshairs();

    glFlush();

    CheckGLError();
}


// ----------------------------------------------------------------------------
// MyApp: the application object
// ----------------------------------------------------------------------------

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
    if ( !wxApp::OnInit() )
        return false;

    new MyFrame();

    return true;
}

int MyApp::OnExit()
{
    delete m_glContext;
    delete m_glStereoContext;

    return wxApp::OnExit();
}

TestGLContext& MyApp::GetContext(wxGLCanvas *canvas)
{
    TestGLContext *glContext;
    // if ( useStereo )
    // {
    //     if ( !m_glStereoContext )
    //     {
    //         // Create the OpenGL context for the first stereo window which needs it:
    //         // subsequently created windows will all share the same context.
    //         m_glStereoContext = new TestGLContext(canvas);
    //     }
    //     glContext = m_glStereoContext;
    // }
    // else
    // {
        if ( !m_glContext )
        {
            // Create the OpenGL context for the first mono window which needs it:
            // subsequently created windows will all share the same context.
            m_glContext = new TestGLContext(canvas);
        }
        glContext = m_glContext;
    // }

    glContext->SetCurrent(*canvas);

    return *glContext;
}

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
    m_current_drawnum =0;
    m_currMousePos = new Point();

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
    // This is required even though dc is not used otherwise.
    wxPaintDC dc(this);

    // Set the OpenGL viewport according to the client size of this canvas.
    // This is done here rather than in a wxSizeEvent handler because our
    // OpenGL rendering context (and thus viewport setting) is used with
    // multiple canvases: If we updated the viewport in the wxSizeEvent
    // handler, changing the size of one canvas causes a viewport setting that
    // is wrong when next another canvas is repainted.
    const wxSize ClientSize = GetClientSize();

    TestGLContext& canvas = wxGetApp().GetContext(this);
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
        canvas.DrawRotatedCube(m_xangle, m_yangle);
        if ( m_useStereo && !m_stereoWarningAlreadyDisplayed )
        {
            m_stereoWarningAlreadyDisplayed = true;
            wxLogError("Stereo not supported by the graphics card.");
        }
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
    Update();  // immediately update the window

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




// ----------------------------------------------------------------------------
// MyFrame: main application window
// ----------------------------------------------------------------------------

// wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
//     EVT_MENU(wxID_NEW, MyFrame::OnNewWindow)
//     EVT_MENU(DRAW_ONE, MyFrame::OnDrawOne)
//     EVT_MENU(DRAW_TWO, MyFrame::OnDrawTwo)
//     //EVT_MENU(NEW_STEREO_WINDOW, MyFrame::OnNewStereoWindow)
//     EVT_MENU(wxID_CLOSE, MyFrame::OnClose)
// wxEND_EVENT_TABLE()

// MemberSelectFrame::MemberSelectFrame()
//        : wxFrame(NULL, wxID_ANY, wxT("wxWidgets OpenGL Cube Sample"))
// {
//    Show();                       // show the frame
// }

MyFrame::MyFrame( bool stereoWindow )
       : wxFrame(NULL, wxID_ANY, wxT("wxWidgets OpenGL Cube Sample"))
{

    int stereoAttribList[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_STEREO, 0 };

    m_canvas = new TestGLCanvas(this, stereoWindow ? stereoAttribList : NULL);
    m_staticlabel = new wxStaticText( this, wxID_ANY, "Example", 
        wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);

    //SetIcon(wxICON(sample));

    // Make a menubar
    wxMenu *menu = new wxMenu;
    menu->Append(wxID_NEW);
    menu->Append(wxID_OPEN);
    menu->Append(wxID_SAVE);
    menu->Append(wxID_SAVEAS);
    menu->AppendSeparator();
    menu->Append(wxID_CLOSE);
    menu->AppendSeparator();

    wxMenu *menu_draw = new wxMenu;
    menu_draw->Append(DRAW_SELECT,wxT("Select Shape"));
    menu_draw->Append(DRAW_ONE,wxT("One"));
    menu_draw->Append(DRAW_TWO,wxT("Two"));

    wxMenu *menu_help = new wxMenu;
    menu_help->Append(wxID_ABOUT);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menu, wxT("&File"));
    menuBar->Append(menu_draw, wxT("&Draw"));
    menuBar->Append(menu_help, wxT("&Help"));
    SetMenuBar(menuBar);

    CreateStatusBar();

    //SetClientSize(400, 400);

    //SetMinSize(wxSize(1000, 600));

    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);          // sizer for the OpenGL and text box components
    wxBoxSizer *drawingsizer = new wxBoxSizer(wxHORIZONTAL);    // the master sizer for the window

    topsizer->Add(
           m_canvas,
           0,              //makes unstretchable in direction of the sizer
           wxEXPAND |      // makes expandable in opposite direction of the sizer
           wxALL,          // makes border all around
           10              // set border width
    );

    topsizer->Add(
           m_staticlabel,
           0,              // makes unstretchable in direction of the sizer
           wxEXPAND |      // makes expandable in opposite direction of the sizer
           wxLEFT | wxRIGHT,          // makes border all around
           10               // set border width
    );

    topsizer->Add(
           new wxTextCtrl( this, -1, "My text.", wxDefaultPosition, wxSize(600,40), wxTE_MULTILINE),
           0,              // makes unstretchable in direction of the sizer
           wxEXPAND |      // makes expandable in opposite direction of the sizer
           wxALL,          // makes border all around
           10              // set border width
    );
    
    drawingsizer->Add(
           topsizer,
           0,
           wxALIGN_CENTER
    );
    SetSizerAndFit(drawingsizer); // resizes and fits all the sizers for our frame dimensions
    Show();                       // show the frame

    // test IsDisplaySupported() function:
    static const int attribs[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, 0 };
    wxLogStatus("Double-buffered display %s supported",
                wxGLCanvas::IsDisplaySupported(attribs) ? "is" : "not");

    // if ( stereoWindow )
    // {
    //     const wxString vendor = glGetwxString(GL_VENDOR).Lower();
    //     const wxString renderer = glGetwxString(GL_RENDERER).Lower();
    //     if ( vendor.find("nvidia") != wxString::npos &&
    //             renderer.find("quadro") == wxString::npos )
    //         ShowFullScreen(true);
    // }

    Connect(wxID_NEW, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(MyFrame::OnNewWindow));
    Connect(wxID_CLOSE, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(MyFrame::OnClose));  
    Connect(DRAW_SELECT, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(MyFrame::OnDrawSelect));  
    Connect(DRAW_ONE, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(MyFrame::OnDrawOne));   
    Connect(DRAW_TWO, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(MyFrame::OnDrawTwo)); 
    Connect(wxID_ABOUT, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(MyFrame::OnAbout));
    Connect(wxID_SAVE, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(MyFrame::OnSave));
    Connect(wxID_SAVEAS, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(MyFrame::OnSaveAs));
    Connect(wxID_FILE, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(MyFrame::OnFile));
}

void MyFrame::OnClose(wxCommandEvent& WXUNUSED(event))
{
    // true is to force the frame to close
    Close(true);
}

void IncompleteAction() {
          wxMessageBox( "INCOMPLETE ACTION",
                 "This action is not complete", wxOK | wxICON_INFORMATION ); 
}

void MyFrame::OnNewWindow( wxCommandEvent& WXUNUSED(event) )
{
    new MyFrame();
}

void MyFrame::OnDrawSelect( wxCommandEvent& WXUNUSED(event) )
{
  DrawMenuDialog dialog = DrawMenuDialog(wxT("Select shape"));

  if (dialog.getSelected() == wxID_OK)
    std::cout << "OK -- so do stuff now" << std::endl;
  else
    std::cout << "Nothing to do...." << std::endl;
//  else if (draw_menu_select == wxID_CLOSE)
//    std::cout << "CANCELLED" << std::endl;
//  else
//    std::cout << "Another option selected" << std::endl;
    dialog.Destroy();
}

void MyFrame::OnDrawOne( wxCommandEvent& WXUNUSED(event) )
{
    m_canvas->setCurrentDrawNum(0); 
    m_canvas->Refresh();  // mark the frame as dirty
    m_canvas->Update();  // immediately update the window
}

void MyFrame::OnDrawTwo( wxCommandEvent& WXUNUSED(event) )
{
    m_canvas->setCurrentDrawNum(1); 
    m_canvas->Refresh();
    m_canvas->Update();
}



void MyFrame::OnFile(wxCommandEvent& WXUNUSED(event) )
{
    IncompleteAction();
}

void MyFrame::OnSave(wxCommandEvent& WXUNUSED(event) )
{
    IncompleteAction();
}

void MyFrame::OnSaveAs(wxCommandEvent& WXUNUSED(event) )
{
    IncompleteAction();
}



void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event) )
{
       wxMessageBox( "SketcherApp v1.0 - by James Allen.\nCopyright 2016",
                 "About SketcherApp", wxOK | wxICON_INFORMATION );
}
