#pragma once
#include "GraphicsContext.h"
#include <Windows.h>
#include <glad\glad.h>
#include <glad\glad_wgl.h>

class COpenGLGraphicsContext : public IGraphicsContext
{
public:
	COpenGLGraphicsContext();
	COpenGLGraphicsContext(EGraphicsAPI api);
	COpenGLGraphicsContext(EGraphicsAPI api, HWND hwnd);
	~COpenGLGraphicsContext();

	virtual void Test() override;
	virtual void SwapImages() override;

	virtual IShader* CreateShader(MCreateShaderProgramInfo info) override;
private:
	HDC m_Hdc;
	HGLRC m_Hglrc;

	// -- Private Helper functions -- //
	void SetDeviceContextFormat(PIXELFORMATDESCRIPTOR pfd, HDC hdc);
	HGLRC CreateFalseContext(HDC hdc);
	HGLRC CreateActualContext(HDC hdc);

};

void DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);