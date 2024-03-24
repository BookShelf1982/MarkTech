#include "OpenGLGraphicsContext.h"

COpenGLGraphicsContext::COpenGLGraphicsContext()
{
}

COpenGLGraphicsContext::COpenGLGraphicsContext(EGraphicsAPI api, HWND hwnd)
{
	HDC hdc = GetDC(hwnd);
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
		PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
		32,                   // Colordepth of the framebuffer.
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,                   // Number of bits for the depthbuffer
		8,                    // Number of bits for the stencilbuffer
		0,                    // Number of Aux buffers in the framebuffer.
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	SetDeviceContextFormat(pfd, hdc);

	HGLRC hglrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hglrc);

	m_Hdc = hdc;
	m_Hglrc = hglrc;

	int status = gladLoadGL();
	MASSERT(status != 0);
}

COpenGLGraphicsContext::~COpenGLGraphicsContext()
{
	wglMakeCurrent(m_Hdc, NULL);
	wglDeleteContext(m_Hglrc);
}

void COpenGLGraphicsContext::Test()
{
	glClearColor(0.0f, 0.0f, 0.45f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void COpenGLGraphicsContext::SwapImages()
{
	SwapBuffers(m_Hdc);
}

void COpenGLGraphicsContext::SetDeviceContextFormat(PIXELFORMATDESCRIPTOR pfd, HDC hdc)
{
	int nPixelFormat;

	nPixelFormat = ChoosePixelFormat(hdc, &pfd);

	SetPixelFormat(hdc, nPixelFormat, &pfd);
}
