#pragma once
#include "GraphicsContext.h"
#include <Windows.h>
#include <glad\glad.h>

class COpenGLGraphicsContext : public IGraphicsContext
{
public:
	COpenGLGraphicsContext();
	COpenGLGraphicsContext(EGraphicsAPI api);
	COpenGLGraphicsContext(EGraphicsAPI api, HWND hwnd);
	~COpenGLGraphicsContext();

	virtual void Test() override;
	virtual void SwapImages() override;
private:
	HDC m_Hdc;
	HGLRC m_Hglrc;

	// -- Private Helper functions -- //
	void SetDeviceContextFormat(PIXELFORMATDESCRIPTOR pfd, HDC hdc);
};