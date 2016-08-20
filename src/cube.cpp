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

//#include "../include/TestGLCanvas.h"
//#include "../include/TestGLCanvas.h"
#include "../include/cube.h"
#include "../include/DrawMenuDialog.h"
#include "../include/TestGLContext.h"
#include "../include/TestGLCanvas.h"
#include "../include/DrawObject.h"
#include "../include/Camera.h"


// ----------------------------------------------------------------------------
// MyApp: the application object
// ----------------------------------------------------------------------------

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
    if ( !wxApp::OnInit() )
        return false;

    mainFrame = new MyFrame();

    return true;
}

int MyApp::OnExit()
{
    //delete m_glContext;

    return wxApp::OnExit();
}

// TestGLContext& MyFrame::GetContext(wxGLCanvas *canvas)
// {
//     //TestGLContext *glContext;

//     if ( !m_glContext )
//     {
//         // Create the OpenGL context for the first mono window which needs it:
//         // subsequently created windows will all share the same context.
//         m_glContext = new TestGLContext(canvas);
//     }

//     m_glContext->SetCurrent(*canvas);

//     return *m_glContext;
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

    m_canvas = new TestGLCanvas(this, NULL);
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
    menu_draw->AppendSeparator();
    menu_draw->Append(VIEW_ONE, wxT("View One"));
    menu_draw->Append(VIEW_TWO, wxT("View Two"));

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

    m_canvas->CreateDrawObj();

    // if(m_canvas->m_crosshair == NULL) {
    //     m_canvas->GetContext(m_canvas);
    //     m_canvas->m_crosshair = new Crosshairs(m_canvas);
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
    Connect(VIEW_ONE, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(MyFrame::OnViewOne));
    Connect(VIEW_TWO, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(MyFrame::OnViewTwo));
}

void MyFrame::OnClose(wxCommandEvent& WXUNUSED(event))
{
    // true is to force the frame to close
    Close(true);
//    delete m_glContext;
    delete m_canvas;
    delete m_staticlabel;
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

void MyFrame::OnViewOne( wxCommandEvent& WXUNUSED(event) ) {
    m_canvas->getCamera()->setPos(glm::vec3(0.0f, 0.0f, 1.0f));
    m_canvas->Refresh();  // mark the frame as dirty
    m_canvas->Update();  // immediately update the window
}

void MyFrame::OnViewTwo( wxCommandEvent& WXUNUSED(event) ) {
    m_canvas->getCamera()->setPos(glm::vec3(0.0f, 0.0f, 15.0f));
    m_canvas->Refresh();  // mark the frame as dirty
    m_canvas->Update();  // immediately update the window
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

