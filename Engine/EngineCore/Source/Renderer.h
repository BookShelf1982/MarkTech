#pragma once
#include "Core.h"
#include "Window.h"
#include "Level.h"
#include "Vectors.h"

struct MObjectConstBuffer
{
	DirectX::XMMATRIX WorldMatrix;
	DirectX::XMMATRIX WorldViewPorjection;
};

struct MWorldConstBuffer
{
	DirectX::XMFLOAT4 SunPos;
};

struct MGenericVertex
{
	MGenericVertex(float x, float y, float z, float u, float v, float nx, float ny, float nz) : pos(x, y, z), tcoords(u, v), norm(nx, ny, nz) {}
	MVector3 pos;
	MVector3 norm;
	MVector2 tcoords;
};

class IVertexBuffer
{
public:
	virtual ~IVertexBuffer() {}
	virtual void ReleaseBuffer() = 0;
};

class IIndexBuffer
{
public:
	virtual ~IIndexBuffer() {}
	virtual void ReleaseBuffer() = 0;
	virtual inline size_t GetInds() const = 0;
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

class IShader
{
public:
	virtual ~IShader() {}
	virtual void ReleaseShader() = 0;
};

enum class ERendererAPI
{
	None = 0,
	Direct3D11 = 1,
};

enum class EBufferUsage
{
	Static = 0,
	Dynamic = 1
};;

class IRenderer
{
public:
	virtual	bool InitRenderer() = 0;
	virtual	void RenderFrame() = 0;
	virtual	void ShutdownRenderer() = 0;
	virtual void SetWindow(IWindow* pWindow) = 0;
	virtual void SetLevel(CLevel* pLevel) = 0;
	virtual IVertexBuffer* CreateVertexBuffer(void* vertexData, size_t vertexDataSize, EBufferUsage bufferUsage) = 0;
	virtual IIndexBuffer* CreateIndexBuffer(void* indexData, size_t indexDataSize, EBufferUsage bufferUsage) = 0;
	virtual void BindVertexBuffer(IVertexBuffer* pVertexBuffer) const = 0;
	virtual void BindIndexBuffer(IIndexBuffer* pVertexBuffer) const = 0;
	virtual IShader* CreateVertexShader(const void* pShaderData, size_t nShaderDataSize) = 0;
	virtual IShader* CreatePixelShader(const void* pShaderData, size_t nShaderDataSize) = 0;
	virtual IConstantBuffer* CreateConstantBuffer(size_t bufferSize) = 0;
	virtual void UpdateConstantBuffer(IConstantBuffer* pBuffer, void* pData) = 0;
	virtual void BindVertexShader(IShader * pShader) const = 0;
	virtual void BindPixelShader(IShader* pShader) const = 0;
	virtual void BindVertConstantBuffer(size_t nSlot, IConstantBuffer* pBuffer) = 0;
	virtual void BindPixelConstantBuffer(size_t nSlot, IConstantBuffer* pBuffer) = 0;
	virtual void SubmitMesh(IVertexBuffer* pVertex, IIndexBuffer* pIndex, IShader* pVertexShader, IShader* pPixelShader) = 0;
	virtual void FinishCommandQueue() = 0;
	inline static ERendererAPI GetCurrentAPI() { return IRenderer::m_API; }
protected:
	static ERendererAPI m_API;
	uint32_t m_nVertexStride = sizeof(MGenericVertex);
	uint32_t m_nVertexOffset = 0;
};