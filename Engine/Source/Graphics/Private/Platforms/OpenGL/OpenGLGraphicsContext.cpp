#include "OpenGLGraphicsContext.h"
#include "OpenGLShader.h"

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

	m_Hglrc = CreateFalseContext(hdc);

	int status = gladLoadGL();
	MASSERT(status != 0);

	status = gladLoadWGL(hdc);
	MASSERT(status != 0);

	m_Hdc = hdc;
	HGLRC finalGlrc = CreateActualContext(hdc);

	wglMakeCurrent(hdc, NULL);
	wglDeleteContext(m_Hglrc);

	m_Hglrc = finalGlrc;

	wglMakeCurrent(hdc, m_Hglrc);

	glDebugMessageCallback(DebugCallback,  nullptr);

	OutputDebugStringA((char*)glGetString(GL_VERSION));
}

COpenGLGraphicsContext::~COpenGLGraphicsContext()
{
	wglMakeCurrent(m_Hdc, NULL);
	wglDeleteContext(m_Hglrc);
}

void COpenGLGraphicsContext::Test()
{
	glClearColor(0.0f, 0.0f, 0.45f, 10.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
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

IShader* COpenGLGraphicsContext::CreateShader(MCreateShaderProgramInfo info)
{
	return new COpenGLShader(info);
}

HGLRC COpenGLGraphicsContext::CreateFalseContext(HDC hdc)
{
	HGLRC hglrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hglrc);
	return hglrc;
}

HGLRC COpenGLGraphicsContext::CreateActualContext(HDC hdc)
{
	const int attribList[] =
	{
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB, 32,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,
		0, // End
	};

	int pixelFormat;
	UINT numFormats;

	wglChoosePixelFormatARB(hdc, attribList, NULL, 1, &pixelFormat, &numFormats);

	const int contextAttribList[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 6,
		GL_CONTEXT_PROFILE_MASK, GL_CONTEXT_CORE_PROFILE_BIT,
		0, // End
	};
	HGLRC hglrc = wglCreateContextAttribsARB(hdc, NULL, contextAttribList);

	return hglrc;
}

void DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	OutputDebugStringA(message);
}
