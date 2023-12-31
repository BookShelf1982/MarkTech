#pragma once
#include "Core.h"
#include "Window.h"

struct MGenericVertex
{
	MGenericVertex(float x, float y, float z) : x(x), y(y), z(z) {}
	float x;
	float y;
	float z;
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
	virtual void BindBuffer() const = 0;
	virtual void UnbindBuffer() const = 0;
};

enum class ERendererAPI
{
	None = 0,
	Direct3D11 = 1,
};

class IRenderer
{
public:
	virtual	bool InitRenderer() = 0;
	virtual	void RenderFrame() = 0;
	virtual	void ShutdownRenderer() = 0;
	virtual void SetWindow(IWindow* pWindow) = 0;
	virtual IVertexBuffer* CreateVertexBuffer(MGenericVertex* vertexData, size_t vertexDataSize) = 0;
	virtual void BindVertexBuffer(IVertexBuffer* pVertexBuffer) const = 0;
	inline static ERendererAPI GetCurrentAPI() { return IRenderer::m_API; }
protected:
	static ERendererAPI m_API;
	uint32_t m_nVertexStride = sizeof(MGenericVertex);
	uint32_t m_nVertexOffset = 0;
};