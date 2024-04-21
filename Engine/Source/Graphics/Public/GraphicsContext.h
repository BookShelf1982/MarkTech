#pragma once
#include "Core.h"
#include "Buffer.h"
#include "Shader.h"
#include "Pipeline.h"

enum class EGraphicsAPI
{
	OpenGL
};

enum class EDrawMode
{
	TRIANGLES
};

class IGraphicsContext
{
public:
	IGraphicsContext() {}
	IGraphicsContext(EGraphicsAPI api) {}
	virtual ~IGraphicsContext() {}

	virtual void Test() = 0;
	virtual void SwapImages() = 0;

	virtual IShader* CreateShader(MCreateShaderInfo info) = 0;
	virtual IBuffer* CreateBuffer(MCreateBufferInfo info) = 0;
	virtual IPipeline* CreatePipeline(MCreatePipelineInfo info) = 0;

	virtual void BindBuffer(IBuffer* pBuffer, EBindingTarget type) = 0;
	virtual void BindPipeline(IPipeline* pPipeline) = 0;
	virtual void DrawVertices(EDrawMode drawMode, uint32_t nStartIndex, uint32_t nNumVerts) = 0;
};