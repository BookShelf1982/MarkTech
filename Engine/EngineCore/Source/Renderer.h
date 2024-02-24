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

class IImage
{
public:
	virtual ~IImage() {}
	virtual void ReleaseImage() = 0;
};

class IImageView
{
public:
	virtual ~IImageView() {}
	virtual void ReleaseImageView() = 0;
};

class IImageSampler
{
public:
	virtual ~IImageSampler() {}
	virtual void ReleaseSampler() = 0;
};

struct MDescriptorSetLayoutBindingDesc
{
	uint32_t binding;
	uint32_t type;
	uint32_t descriptorCount;
	uint32_t stageFlags;
};

struct MDescriptorSetLayoutDesc
{
	uint32_t bindingCount;
	MDescriptorSetLayoutBindingDesc* pBindings;
};

enum EDescriptorType
{
	UNIFORM_BUFFER,
	IMAGE_SAMPLER
};

struct MDescriptorBufferDesc
{
	IConstantBuffer* pBuffer;
	size_t offset;
	size_t range;
};

struct MDesciptorImageDesc
{
	uint32_t imageLayout;
	IImageView* pImageView;
	IImageSampler* pImageSampler;
};

struct MDescriptorDesc
{
	uint32_t binding;
	EDescriptorType type;
	MDescriptorBufferDesc* pBufferInfo;
	MDesciptorImageDesc* pImageInfo;
};

class IDescriptorSet
{
public:
	virtual ~IDescriptorSet() {}
	virtual void ReleaseDescriptorSet() = 0;
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

	virtual IImage* CreateImage(char* data, size_t dataSize, size_t width, size_t height) = 0;

	virtual IImageView* CreateImageView(IImage* image) = 0;

	virtual IImageSampler* CreateImageSmpler(IImageView* view) = 0;

	virtual IConstantBuffer* CreateConstantBuffer(size_t dataSize, EUsageType usage) = 0;

	virtual IDescriptorSet* CreateDescriptorSet(MDescriptorDesc* setDescs, size_t setDescsCount, MDescriptorSetLayoutDesc layoutDesc) = 0;
	virtual void UpdateConstantBuffer(IConstantBuffer* constantBuffer, void* data, size_t size) = 0;

	virtual IPipelineObject* CreatePipeline(
		IShader* vertexShader, 
		IShader* fragmentShader, 
		IDescriptorSet** descriptorSets,
		size_t descriptorSetsCount
	) = 0;

	// -- Command Buffer Funcs -- //
	virtual void BeginCommandRecording() = 0;
	virtual void EndCommandRecording() = 0;
	virtual void SubmitCommandRecording() = 0;
	virtual void BindPipelineObject(IPipelineObject* pipeline) = 0;
	virtual void BindVertexBuffer(IBuffer* buffer, size_t offset) = 0;
	virtual void BindIndexBuffer(IBuffer* buffer, size_t offset) = 0;
	virtual void BindDescriptorSets(IDescriptorSet** descriptorSets, size_t descriptorSetsCount) = 0;
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