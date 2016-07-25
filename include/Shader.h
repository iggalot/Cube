/////////////////////////////////////////////////////////////////////////////
// Name:        DrawObject.h
// Controls the assembly and render of various drawing objects used by the
// draw routines in the TestGLContext class
/////////////////////////////////////////////////////////////////////////////

#ifndef _SHADER_H
#define _SHADER_H

class Shader {
public:
	Shader(const GLchar* vsource, const GLchar* fsource);

	// method
	void CreateProgram();
	void Load();
	void Unload();

private:
	const GLchar* fshadesource;
	const GLchar* vshadesource;
	GLuint m_ProgramNum;
};

#endif // _SHADER_H
