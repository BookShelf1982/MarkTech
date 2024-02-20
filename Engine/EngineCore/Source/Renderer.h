#pragma once
#include "Core.h"
#include "Window.h"
#include "Vectors.h"

#define FRAME_BUFFER_COUNT 2

struct MGenericVertex
{
	MGenericVertex(float x, float y, float z, float u, float v, float nx, float ny, float nz) : pos(x, y, z), tcoords(u, v), norm(nx, ny, nz) {}
	MVector3 pos;
	MVector3 norm;
	MVector2 tcoords;
};

class IBuffer
{
public:
	virtual ~IBuffer() {}
	virtual void ReleaseBuffer() = 0;
};

class IConstantBuffer
{
public:
	virtual ~IConstantBuffer() {}
	virtual void ReleaseBuffer() = 0;
};

enum class EShaderType
{
	Unkown = 0,
	Vertex = 1,
	Pixel = 2
};

enum class EUsageType
{
	VertexShader = 0,
	PixelShader = 1
};

class IShader
{
public:
	virtual ~IShader() {}
	virtual void ReleaseShader() = 0;
};

class IPipelineObject
{
public:
	virtual ~IPipelineObject() {}
	virtual void Release() = 0;
};

enum class ERendererAPI
{
	None = 0,
	Direct3D11 = 1,
	Direct3D12 = 2,
	Vulkan = 3
};

enum class EBufferUsage
{
	Static = 0,
	Dynamic = 1
};

struct MViewport
{
	float TopLeftX;
	float TopLeftY;
	float Width;
	float Height;
	float MinDepth;
	float MaxDepth;
};

struct MRect
{
	uint32_t left;
	uint32_t top;
	uint32_t front;
	uint32_t right;
	uint32_t bottom;
	uint32_t back;
};

class IRenderer
{
public:
	virtual ~IRenderer() {}
	virtual	bool InitRenderer(IWindow* window) = 0;
	virtual	void ShutdownRenderer() = 0;
	inline static ERendererAPI GetCurrentAPI() { return IRenderer::m_API; }

	virtual IShader* CreateShader(char* data, size_t dataSize) = 0;

	virtual IBuffer* CreateBuffer(char* data, size_t dataSize) = 0;

	virtual IConstantBuffer* CreateConstantBuffer(size_t dataSize, EUsageType usage) = 0;
	virtual void UpdateConstantBuffer(IConstantBuffer* constantBuffer, void* data, size_t size) = 0;

	virtual IPipelineObject* CreatePipeline(IShader* vertexShader, IShader* fragmentShader, IConstantBuffer** constantBuffers, size_t constantBufferCount) = 0;

	// -- Command Buffer Funcs -- //
	virtual void BeginCommandRecording() = 0;
	virtual void EndCommandRecording() = 0;
	virtual void SubmitCommandRecording() = 0;
	virtual void BindPipelineObject(IPipelineObject* pipeline) = 0;
	virtual void BindVertexBuffer(IBuffer* buffer, size_t offset) = 0;
	virtual void BindIndexBuffer(IBuffer* buffer, size_t offset) = 0;
	virtual void BindConstantBuffer(IConstantBuffer* buffer) = 0;
	virtual void SetViewportRect(MViewport viewport) = 0;
	virtual void SetScissorRect(MRect rect) = 0;
	virtual void DrawVertices(uint32_t numVerts) = 0;
	virtual void DrawIndices(uint32_t numInds) = 0;

	// -- Sync -- //
	virtual void WaitForPreviousFrame() = 0;
	virtual void WaitForDeviceToIdle() = 0;

	// -- Swapchain -- //
	virtual void AquireNextSwapChainImage() = 0;
	virtual void Present() = 0;
protected:
	static ERendererAPI m_API;
};