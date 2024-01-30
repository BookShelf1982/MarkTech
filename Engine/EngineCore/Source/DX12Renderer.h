#pragma once
#include "Renderer.h"
#include "WinWindow.h"
#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include "d3dx12.h"
#pragma comment(lib, "d3d12.lib")
#pragma comment( lib, "dxgi.lib" )
#pragma comment( lib, "dxguid.lib" )

#define FRAME_BUFFER_COUNT 2

// this will only call release if an object exists (prevents exceptions calling release on non existant objects)
#define SAFE_RELEASE(p) { if ( (p) ) { (p)->Release(); (p) = 0; } }

class CDX12Renderer : public IRenderer
{
public:
	virtual	bool InitRenderer(IWindow* Window) override;
	virtual	void ShutdownRenderer() override;
	virtual void SetInitialVertexShaderBytecode(const char* pData, size_t size) override;
	virtual void WaitForPreviousFrame() override;

	virtual IPipelineState* CreatePipelineStateObject(const char* vertexShdaerData, size_t vertexShaderDataSize, const char* pixelShdaerData, size_t pixelShaderDataSize) override;
	virtual IVertexBuffer* CreateVertexBuffer(void* vertexData, size_t vertexDataSize, EBufferUsage bufferUsage) override;
	virtual IIndexBuffer* CreateIndexBuffer(void* indexData, size_t indexDataSize, EBufferUsage bufferUsage) override;
	virtual void BindVertexBuffer(IVertexBuffer* pVertexBuffer) const override;
	virtual void BindIndexBuffer(IIndexBuffer* pIndexBuffer) const override;
	virtual IShader* CreateVertexShader(const void* pShaderData, size_t nShaderDataSize) override;
	virtual IShader* CreatePixelShader(const void* pShaderData, size_t nShaderDataSize) override;
	virtual IConstantBuffer* CreateConstantBuffer(size_t nBufferSize) override;
	virtual void DrawVertices(size_t numVerts) override;
	virtual void DrawIndices(size_t numVerts) override;
	virtual void SetViewport(MViewport viewport, MRect scissorView) override;
	virtual void BindCurrentBackBufferAsRenderTarget() override;
	virtual void PresentBackBuffer() override;
	virtual void SwapBackBuffers() override;
	virtual void ClearRenderTarget(float color[4]) override;
	virtual void UpdateConstantBuffer(IConstantBuffer* pBuffer, void* pData) override;
	virtual void BindPipelineStateObject(IPipelineState* pPipelineState) override;
	virtual void BindVertexShader(IShader* pShader) const override;
	virtual void BindPixelShader(IShader* pShader) const override;
	virtual void BindVertConstantBuffer(size_t nSlot, IConstantBuffer* pBuffer) override;
	virtual void BindPixelConstantBuffer(size_t nSlot, IConstantBuffer* pBuffer) override;
	virtual void SubmitMesh(IVertexBuffer* pVertex, IIndexBuffer* pIndex, IShader* pVertexShader, IShader* pPixelShader) override;
	virtual void RestartCommandQueue() override;
	virtual void ExecuteCommandQueue() override;
	virtual void FinishCommandQueue() override;
	virtual void SubmitSceneBuffer(CSceneBuffer buffer) override;
private:
	IWindow* m_pWindowRef;

	#ifdef DEBUG
	ID3D12Debug* m_pDebug;
	#endif // DEBUG

	// direct3d stuff

	D3D12_VIEWPORT m_Viewport;
	D3D12_RECT m_ScissorRect;

	ID3D12Device* m_pDevice; // direct3d device

	IDXGISwapChain3* m_pSwapChain; // swapchain used to switch between render targets

	ID3D12CommandQueue* m_pCommandQueue; // container for command lists

	ID3D12DescriptorHeap* m_pRenderTargetView; // a descriptor heap to hold resources like the render targets

	ID3D12Resource* m_pRenderTargets[FRAME_BUFFER_COUNT]; // number of render targets equal to buffer count

	ID3D12CommandAllocator* m_pCommandAllocator[FRAME_BUFFER_COUNT]; // we want enough allocators for each buffer * number of threads (we only have one thread)

	ID3D12GraphicsCommandList* m_pCommandList; // a command list we can record commands into, then execute them to render the frame

	ID3D12Fence* m_pFences[FRAME_BUFFER_COUNT];    // an object that is locked while our command list is being executed by the gpu. We need as many 
	//as we have allocators (more if we want to know when the gpu is finished with an asset)

	ID3D12RootSignature* m_pRootSignature;

	ID3D12PipelineState* m_pMainPipelineState;

	IVertexBuffer* m_pVertexBuffer;

	ID3D12Heap* m_pUploadHeap;

	HANDLE m_FenceEvent; // a handle to an event when our fence is unlocked by the gpu

	UINT64 m_pFenceValues[FRAME_BUFFER_COUNT]; // this value is incremented each frame. each fence will have its own value

	int nFrameIndex; // current rtv we are on

	int RenderTargetViewSize; // size of the rtv descriptor on the device (all front and back buffers will be the same size)

	D3D12_SHADER_BYTECODE m_pInitialVertexShader;
};

bool CreateDX12Renderer(IRenderer** ppRenderer);

class CDX12VertexBuffer : public IVertexBuffer
{
public:
	CDX12VertexBuffer(ID3D12Resource* pResource, ID3D12Resource* pUploadBuffer, D3D12_VERTEX_BUFFER_VIEW bufferView, uint64_t nId = 0);
	~CDX12VertexBuffer();
	virtual void ReleaseBuffer() override;
	virtual inline uint64_t GetIdFromAssetId() const override { return m_nIdFromAssetId; }
	ID3D12Resource* GetBuffer() const { return m_pVertexBuffer; }
	D3D12_VERTEX_BUFFER_VIEW GetBufferView() const { return m_BufferView; }
private:
	ID3D12Resource* m_pVertexBuffer;
	ID3D12Resource* m_pUploadBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_BufferView;
	uint64_t m_nIdFromAssetId;
};

class CDX12IndexBuffer : public IIndexBuffer
{
public:
	CDX12IndexBuffer(ID3D12Resource* pResource, ID3D12Resource* pUploadBuffer, D3D12_INDEX_BUFFER_VIEW bufferView, uint64_t nId = 0, size_t nNumInds = 0);
	~CDX12IndexBuffer();
	virtual void ReleaseBuffer() override;
	virtual inline size_t GetInds() const { return m_nNumInds; }
	virtual inline uint64_t GetIdFromAssetId() const { return m_nIdFromAssetId; }
	D3D12_INDEX_BUFFER_VIEW GetBufferView() const { return m_BufferView; }
private:
	ID3D12Resource* m_pIndexBuffer;
	ID3D12Resource* m_pUploadBuffer;
	D3D12_INDEX_BUFFER_VIEW m_BufferView;
	uint64_t m_nIdFromAssetId;
	size_t m_nNumInds;
};

class CDX12PipelineState : public IPipelineState
{
public:
	CDX12PipelineState(ID3D12PipelineState* pPipelineState);
	~CDX12PipelineState();
	virtual void ReleaseStateObject() override;
	ID3D12PipelineState* GetPipelineState();
private:
	ID3D12PipelineState* m_pPipelineState;
};