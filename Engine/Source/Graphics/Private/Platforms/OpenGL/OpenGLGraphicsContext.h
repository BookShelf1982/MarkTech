#pragma once
#include "GraphicsContext.h"
#include "OpenGLShader.h"
#include "OpenGLBuffer.h"
#include "OpenGLPipeline.h"
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
	virtual IShader* CreateShader(MCreateShaderInfo info) override;
	virtual IBuffer* CreateBuffer(MCreateBufferInfo info) override;
	virtual IPipeline* CreatePipeline(MCreatePipelineInfo info) override;

	virtual void BindBuffer(IBuffer* pBuffer, EBindingTarget type) override;
	virtual void BindPipeline(IPipeline* pPipeline) override;
	virtual void DrawVertices(EDrawMode drawMode, uint32_t nStartIndex, uint32_t nNumVerts) override;
private:
	HDC m_Hdc;
	HGLRC m_Hglrc;

	// -- Private Helper functions -- //
	void SetDeviceContextFormat(PIXELFORMATDESCRIPTOR pfd, HDC hdc);
	HGLRC CreateFalseContext(HDC hdc);
	HGLRC CreateActualContext(HDC hdc);

	uint32_t m_nDefaultVAO;
};

void GLAPIENTRY
MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam);