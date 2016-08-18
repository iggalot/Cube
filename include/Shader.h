/////////////////////////////////////////////////////////////////////////////
// Name:        DrawObject.h
// Controls the assembly and render of various drawing objects used by the
// draw routines in the TestGLContext class
/////////////////////////////////////////////////////////////////////////////

#ifndef _SHADER_H
#define _SHADER_H

#include <string>

class Shader {
public:
	Shader(std::string vsource = "shaders/default_shader.vert", 
		std::string fsource = "shaders/default_shader.frag");

	// method
//	void CreateProgram();

	void Unload();
	GLuint getShaderProgNum() {return m_ProgramNum;}

private:
	void CreateProgram();
	std::string LoadFromFile(std::string strFile);

	std::string vertexPath;  // store the path and filename of the vertex shader
	std::string fragmentPath; // store the path and filename of the fragment shader

	const char* fshadesource; // the ascii string of our frag shader once it has been read in
	const char* vshadesource; // the ascii string of our vertex shader once it has been read in

	GLuint m_ProgramNum;
};

#endif // _SHADER_H
