#pragma once
#include "Core.h"
#include "Window.h"
#include "Vectors.h"

#define FRAME_BUFFER_COUNT 2

enum class ESceneElementType
{
	Model
};

struct MSceneElement
{
	ESceneElementType type;
	uint64_t assetId;
	DirectX::XMMATRIX transform;
};

class CSceneBuffer
{
public:
	CSceneBuffer();
	~CSceneBuffer();

	void Initialize(size_t initalSize);
	void Release();
	MSceneElement* GetSceneElements() const { return m_pSceneElements; }
	size_t GetNumSceneElements() const { return m_nNumSceneElements; }
	size_t GetNumPushedSceneElements() const { return m_nNumSceneElements; }
	void PushSceneElement(MSceneElement element);
	void Clear();
private:
	MSceneElement* m_pSceneElements;
	size_t m_nNumSceneElements;
	size_t m_nNumPushedSceneElements;
};

struct MObjectConstBuffer
{
	DirectX::XMMATRIX WorldMatrix;
	DirectX::XMMATRIX WorldViewPorjection;
};

struct MWorldConstBuffer
{
	DirectX::XMFLOAT4 SunPos;
};

struct MLambertProps
{
	DirectX::XMFLOAT4 Color;
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
	virtual inline uint64_t GetIdFromAssetId() const = 0;
};

class IIndexBuffer
{
public:
	virtual ~IIndexBuffer() {}
	virtual void ReleaseBuffer() = 0;
	virtual inline size_t GetInds() const = 0;
	virtual inline uint64_t GetIdFromAssetId() const = 0;
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

class IPipelineState
{
public:
	virtual ~IPipelineState() {}
	virtual void ReleaseStateObject() = 0;
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
	virtual	bool InitRenderer(IWindow* window) = 0;
	virtual	void ShutdownRenderer() = 0;
	inline static ERendererAPI GetCurrentAPI() { return IRenderer::m_API; }
protected:
	static ERendererAPI m_API;
	uint32_t m_nVertexStride = sizeof(MGenericVertex);
	uint32_t m_nVertexOffset = 0;
};