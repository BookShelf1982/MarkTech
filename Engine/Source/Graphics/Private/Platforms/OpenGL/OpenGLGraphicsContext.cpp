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

	glGenVertexArrays(1, &m_nVAO);
	glBindVertexArray(m_nVAO);

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);

	char buffer[1024];
	sprintf_s(buffer, "%s\n%s\n", glGetString(GL_VERSION), glGetString(GL_VENDOR));
	OutputDebugStringA(buffer);

}

void GLAPIENTRY
MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	char buffer[1024];
	sprintf_s(buffer, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
	OutputDebugStringA(buffer);
}

COpenGLGraphicsContext::~COpenGLGraphicsContext()
{
	glDeleteVertexArrays(1, &m_nVAO);
	wglMakeCurrent(m_Hdc, NULL);
	wglDeleteContext(m_Hglrc);
}

void COpenGLGraphicsContext::Test()
{
	glClearColor(0.0f, 0.0f, 0.45f, 10.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void COpenGLGraphicsContext::SwapImages()
{
	SwapBuffers(m_Hdc);
}

IShader* COpenGLGraphicsContext::CreateShader(MCreateShaderInfo info)
{
	return new COpenGLShader(info);
}

IBuffer* COpenGLGraphicsContext::CreateBuffer(MCreateBufferInfo info)
{
	return new COpenGLBuffer(info);
}

IPipeline* COpenGLGraphicsContext::CreatePipeline(MCreatePipelineInfo info)
{
	return new COpenGLPipeline(info);
}

void COpenGLGraphicsContext::BindPipeline(IPipeline* pPipeline)
{
	COpenGLPipeline* pGLPipeline = dynamic_cast<COpenGLPipeline*>(pPipeline);
	glUseProgram(pGLPipeline->GetShaderProgramId());

	MInputLayoutInfo input = pGLPipeline->GetInputLayout();
	for (uint32_t i = 0; i < input.nElementCount; i++)
	{
		uint32_t size = 1;
		uint32_t type = GL_FLOAT;
		switch (input.pElements[i].type)
		{
		case EElementType::FLOAT:
			size = 1;
			type = GL_FLOAT;
			break;
		case EElementType::FLOAT2:
			size = 2;
			type = GL_FLOAT;
			break;
		case EElementType::FLOAT3:
			size = 3;
			type = GL_FLOAT;
			break;
		case EElementType::FLOAT4:
			size = 4;
			type = GL_FLOAT;
			break;
		default:
			break;
		}

		glEnableVertexAttribArray(input.pElements[i].nIndex);

		glVertexAttribPointer(
			input.pElements[i].nIndex,
			size,
			type,
			GL_FALSE,
			input.nStride,
			(void*)input.pElements[i].nOffsetInBytes);
	}
}

void COpenGLGraphicsContext::BindBuffer(IBuffer* pBuffer, EBindingTarget type)
{
	uint32_t nBufferType = 0;
	switch (type)
	{
	case EBindingTarget::VERTEX_ARRAY:
		nBufferType = GL_ARRAY_BUFFER;
		break;
	case EBindingTarget::INDEX_ARRAY:
		nBufferType = GL_ELEMENT_ARRAY_BUFFER;
		break;
	case EBindingTarget::UNIFORM_BUFFER:
		nBufferType = GL_UNIFORM_BUFFER;
		break;
	default:
		nBufferType = GL_ARRAY_BUFFER;
		break;
	}

	glBindBuffer(nBufferType, dynamic_cast<COpenGLBuffer*>(pBuffer)->GetId());
}

void COpenGLGraphicsContext::DrawVertices(EDrawMode drawMode, uint32_t nStartIndex, uint32_t nNumVerts)
{
	uint32_t nDrawMode = 0;
	switch (drawMode)
	{
	case EDrawMode::TRIANGLES:
		nDrawMode = GL_TRIANGLES;
		break;
	default:
		nDrawMode = GL_TRIANGLES;
		break;
	}

	glDrawArrays(nDrawMode, nStartIndex, nNumVerts);
}

void COpenGLGraphicsContext::DrawIndexed(EDrawMode drawMode, uint32_t nCount)
{
	uint32_t nDrawMode = 0;
	switch (drawMode)
	{
	case EDrawMode::TRIANGLES:
		nDrawMode = GL_TRIANGLES;
		break;
	default:
		nDrawMode = GL_TRIANGLES;
		break;
	}

	glDrawElements(nDrawMode, nCount, GL_UNSIGNED_INT, 0);
}

void COpenGLGraphicsContext::SetDeviceContextFormat(PIXELFORMATDESCRIPTOR pfd, HDC hdc)
{
	int nPixelFormat;

	nPixelFormat = ChoosePixelFormat(hdc, &pfd);

	SetPixelFormat(hdc, nPixelFormat, &pfd);
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

#ifdef  DEBUG
	const int contextAttribList[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 6,
		GL_CONTEXT_PROFILE_MASK, GL_CONTEXT_CORE_PROFILE_BIT,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
		0, // End
	};
#else
	const int contextAttribList[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 6,
		GL_CONTEXT_PROFILE_MASK, GL_CONTEXT_CORE_PROFILE_BIT,
		0, // End
	};
#endif
	HGLRC hglrc = wglCreateContextAttribsARB(hdc, NULL, contextAttribList);

	return hglrc;
}
