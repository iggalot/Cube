/////////////////////////////////////////////////////////////////////////////
// Name:        cube.h
// Purpose:     wxGLCanvas demo program
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////



#ifndef _TESTGLCONTEXT_H_
#define _TESTGLCONTEXT_H_

#include "wx/glcanvas.h"
#include "../include/Geometry.h"
//#include "../include/TestGLCanvas.h"

//forward declaration
class TestGLCanvas;
//class DrawMenuDialog

// the rendering context used by all GL canvases
class TestGLContext : public wxGLContext
{
public:
    TestGLContext(wxGLCanvas *canvas);

    // render the cube showing it at given angles
    void DrawRotatedCube(float xangle, float yangle);
    void DoDrawCubeOne(float xangle, float yangle);
    void DoDrawCubeTwo(float xangle, float yangle);
 //   void DrawCrosshairs();

private:
    // textures for the cube faces
    TestGLCanvas *orig_canvas;
    GLuint m_textures[6];
};

#endif // _TESTGLCONTEXT_H_
