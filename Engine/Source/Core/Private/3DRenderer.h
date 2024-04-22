#pragma once
#include "Core.h"
#include "Module.h"
#include "GraphicsInterface.h"

struct comp_shader_t
{
	void Release()
	{
		delete[] pData;
	}

	void* pData;
	uint32_t nDataSize;
};

struct Vert
{
	Vert(float x, float y, float z, float r, float g, float b)
		:x(x), y(y), z(z), r(r), g(g), b(b)
	{
	}
	float x, y, z;
	float r, g, b;
};

class C3DRenderer
{
public:
#ifdef MT_PLATFORM_WINDOWS
	C3DRenderer(HWND hwnd);
#endif
	~C3DRenderer();

	void RenderFrame();

	IBuffer* UploadToGPU(MCreateBufferInfo info);
	IShader* CreateShader(MCreateShaderInfo info);
private:
	comp_shader_t C3DRenderer::ReadShaderFromFile(const char* pFilepath);

	IModule* m_pGraphicsModule;
	IGraphicsContext* m_pGraphicsContext;
	graphics_funcs_t m_pfnGraphicsFuncs;

	CTSharedPointer<IBuffer> m_Buffer;
	CTSharedPointer<IBuffer> m_IndexBuffer;
	CTSharedPointer<IShader> m_VertShader;
	CTSharedPointer<IShader> m_FragShader;
	CTSharedPointer<IPipeline> m_Pipeline;
};