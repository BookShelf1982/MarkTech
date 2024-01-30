#include "DX12Renderer.h"

bool CreateDX12Renderer(IRenderer** ppRenderer)
{
    *ppRenderer = new CDX12Renderer();
    return true;
}

bool CDX12Renderer::InitRenderer(IWindow* Window)
{
    m_pWindowRef = Window;

    m_API = ERendererAPI::Direct3D12;

    HRESULT hr;

    // -- Create the Device -- //

    IDXGIFactory4* dxgiFactory;
    hr = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
    if (FAILED(hr))
    {
        return false;
    }

    IDXGIAdapter3* adapter; // adapters are the graphics card (this includes the embedded graphics on the motherboard)

    int adapterIndex = 0; // we'll start looking for directx 12  compatible graphics devices starting at index 0

    bool adapterFound = false; // set this to true when a good one was found

    // find first hardware gpu that supports d3d 12
    while (dxgiFactory->EnumAdapters1(adapterIndex, (IDXGIAdapter1**)&adapter) != DXGI_ERROR_NOT_FOUND)
    {
        DXGI_ADAPTER_DESC1 desc;
        adapter->GetDesc1(&desc);

        if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
        {
            // we dont want a software device
            adapterIndex++;
            continue;
        }

        // we want a device that is compatible with direct3d 12 (feature level 11 or higher)
        hr = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr);
        if (SUCCEEDED(hr))
        {
            adapterFound = true;
            break;
        }

        adapterIndex++;
    }

    if (!adapterFound)
    {
        return false;
    }

#ifdef DEBUG
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&m_pDebug))))
        m_pDebug->EnableDebugLayer();
#endif

    DXGI_QUERY_VIDEO_MEMORY_INFO info;
    hr = adapter->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &info);

    // Create the device
    hr = D3D12CreateDevice(
        adapter,
        D3D_FEATURE_LEVEL_11_0,
        IID_PPV_ARGS(&m_pDevice)
    );
    if (FAILED(hr))
    {
        return false;
    }

    // -- Create a direct command queue -- //

    D3D12_COMMAND_QUEUE_DESC cqDesc = {};
    cqDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    cqDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT; // direct means the gpu can directly execute this command queue

    hr = m_pDevice->CreateCommandQueue(&cqDesc, IID_PPV_ARGS(&m_pCommandQueue)); // create the command queue
    if (FAILED(hr))
    {
        return false;
    }

    // -- Create the Swap Chain (double/tripple buffering) -- //

    DXGI_MODE_DESC backBufferDesc = {}; // this is to describe our display mode
    backBufferDesc.Width = m_pWindowRef->GetWidth(); // buffer width
    backBufferDesc.Height = m_pWindowRef->GetHeight(); // buffer height
    backBufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // format of the buffer (rgba 32 bits, 8 bits for each chanel)

    // describe our multi-sampling. We are not multi-sampling, so we set the count to 1 (we need at least one sample of course)
    DXGI_SAMPLE_DESC sampleDesc = {};
    sampleDesc.Count = 1; // multisample count (no multisampling, so we just put 1, since we still need 1 sample)

    // Describe and create the swap chain.
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = FRAME_BUFFER_COUNT; // number of buffers we have
    swapChainDesc.BufferDesc = backBufferDesc; // our back buffer description
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // this says the pipeline will render to this swap chain
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // dxgi will discard the buffer (data) after we call present
    swapChainDesc.OutputWindow = reinterpret_cast<CWinWindow*>(m_pWindowRef)->GetHWND(); // handle to our window
    swapChainDesc.SampleDesc = sampleDesc; // our multi-sampling description
    swapChainDesc.Windowed = true; // set to true, then if in fullscreen must call SetFullScreenState with true for full screen to get uncapped fps

    IDXGISwapChain* tempSwapChain;

    hr = dxgiFactory->CreateSwapChain(
        m_pCommandQueue, // the queue will be flushed once the swap chain is created
        &swapChainDesc, // give it the swap chain description we created above
        &tempSwapChain // store the created swap chain in a temp IDXGISwapChain interface
    );

    if (FAILED(hr))
        return false;

    m_pSwapChain = static_cast<IDXGISwapChain3*>(tempSwapChain);

    nFrameIndex = m_pSwapChain->GetCurrentBackBufferIndex();

    // -- Create the Back Buffers (render target views) Descriptor Heap -- //

    // describe an rtv descriptor heap and create
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = FRAME_BUFFER_COUNT; // number of descriptors for this heap.
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // this heap is a render target view heap

    // This heap will not be directly referenced by the shaders (not shader visible), as this will store the output from the pipeline
    // otherwise we would set the heap's flag to D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    hr = m_pDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_pRenderTargetView));
    if (FAILED(hr))
    {
        return false;
    }

    // get the size of a descriptor in this heap (this is a rtv heap, so only rtv descriptors should be stored in it.
    // descriptor sizes may vary from device to device, which is why there is no set size and we must ask the 
    // device to give us the size. we will use this size to increment a descriptor handle offset
    RenderTargetViewSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    // get a handle to the first descriptor in the descriptor heap. a handle is basically a pointer,
    // but we cannot literally use it like a c++ pointer.
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_pRenderTargetView->GetCPUDescriptorHandleForHeapStart());

    // Create a RTV for each buffer (double buffering is two buffers, tripple buffering is 3).
    for (int i = 0; i < FRAME_BUFFER_COUNT; i++)
    {
        // first we get the n'th buffer in the swap chain and store it in the n'th
        // position of our ID3D12Resource array
        hr = m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&m_pRenderTargets[i]));
        if (FAILED(hr))
        {
            return false;
        }

        // the we "create" a render target view which binds the swap chain buffer (ID3D12Resource[n]) to the rtv handle
        m_pDevice->CreateRenderTargetView(m_pRenderTargets[i], nullptr, rtvHandle);

        // we increment the rtv handle by the rtv descriptor size we got above
        rtvHandle.Offset(1, RenderTargetViewSize);
    }

    // -- Create the Command Allocators -- //

    for (int i = 0; i < FRAME_BUFFER_COUNT; i++)
    {
        hr = m_pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_pCommandAllocator[i]));
        if (FAILED(hr))
        {
            return false;
        }
    }

    // -- Create a Command List -- //

    // create the command list with the first allocator
    hr = m_pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_pCommandAllocator[0], NULL, IID_PPV_ARGS(&m_pCommandList));
    if (FAILED(hr))
    {
        return false;
    }

    // command lists are created in the recording state. our main loop will set it up for recording again so close it now
    m_pCommandList->Close();

    // -- Create a Fence & Fence Event -- //

    // create the fences
    for (int i = 0; i < FRAME_BUFFER_COUNT; i++)
    {
        hr = m_pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_pFences[i]));
        if (FAILED(hr))
        {
            return false;
        }
        m_pFenceValues[i] = 0; // set the initial fence value to 0
    }

    // create a handle to a fence event
    m_FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (m_FenceEvent == nullptr)
    {
        return false;
    }

    // -- Create Root Singanture -- //

    CD3DX12_ROOT_SIGNATURE_DESC rsDesc;
    rsDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
    ID3DBlob* pRootSingature;
    hr = D3D12SerializeRootSignature(&rsDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pRootSingature, nullptr);
    if (FAILED(hr))
        return false;

    hr = m_pDevice->CreateRootSignature(0, pRootSingature->GetBufferPointer(), pRootSingature->GetBufferSize(), IID_PPV_ARGS(&m_pRootSignature));
    if (FAILED(hr))
        return false;

    return true;
}

void CDX12Renderer::ShutdownRenderer()
{
    // get swapchain out of full screen before exiting
    BOOL fs = false;
    if (m_pSwapChain->GetFullscreenState(&fs, NULL))
        m_pSwapChain->SetFullscreenState(false, NULL);

    SAFE_RELEASE(m_pDevice);
    SAFE_RELEASE(m_pSwapChain);
    SAFE_RELEASE(m_pCommandQueue);
    SAFE_RELEASE(m_pRenderTargetView);
    SAFE_RELEASE(m_pCommandList);
    SAFE_RELEASE(m_pRootSignature);
    SAFE_RELEASE(m_pUploadHeap);
    if (m_pVertexBuffer)
        m_pVertexBuffer->ReleaseBuffer();

    for (int i = 0; i < FRAME_BUFFER_COUNT; ++i)
    {
        SAFE_RELEASE(m_pRenderTargets[i]);
        SAFE_RELEASE(m_pCommandAllocator[i]);
        SAFE_RELEASE(m_pFences[i]);
    };

#ifdef DEBUG
    SAFE_RELEASE(m_pDebug);
#endif // DEBUG

}

IPipelineState* CDX12Renderer::CreatePipelineStateObject(const char* vertexShdaerData, size_t vertexShaderDataSize, const char* pixelShdaerData, size_t pixelShaderDataSize)
{
    HRESULT hr;
    ID3D12PipelineState* pPipelineState = NULL;

    D3D12_INPUT_ELEMENT_DESC inputDesc[] = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
        {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
    };

    D3D12_INPUT_LAYOUT_DESC ilDesc;
    ilDesc.pInputElementDescs = inputDesc;
    ilDesc.NumElements = sizeof(inputDesc) / sizeof(D3D12_INPUT_ELEMENT_DESC);

    DXGI_SAMPLE_DESC sampleDesc;
    sampleDesc.Count = 1;
    sampleDesc.Quality = 0;

    D3D12_SHADER_BYTECODE vertexShader;
    vertexShader.pShaderBytecode = vertexShdaerData;
    vertexShader.BytecodeLength = vertexShaderDataSize;

    D3D12_SHADER_BYTECODE pixelShader;
    pixelShader.pShaderBytecode = pixelShdaerData;
    pixelShader.BytecodeLength = pixelShaderDataSize;

    D3D12_GRAPHICS_PIPELINE_STATE_DESC psDesc = {};
    psDesc.InputLayout = ilDesc;
    psDesc.pRootSignature = m_pRootSignature;
    psDesc.VS = vertexShader;
    psDesc.PS = pixelShader;
    psDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    psDesc.SampleDesc = sampleDesc;
    psDesc.SampleMask = 0xffffffff;
    psDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    psDesc.NumRenderTargets = 1;

    hr = m_pDevice->CreateGraphicsPipelineState(&psDesc, IID_PPV_ARGS(&pPipelineState));
    if (FAILED(hr))
        return new CDX12PipelineState(NULL);

    return new CDX12PipelineState(pPipelineState);
}

IVertexBuffer* CDX12Renderer::CreateVertexBuffer(void* vertexData, size_t vertexDataSize, EBufferUsage bufferUsage)
{
    WaitForPreviousFrame();
    RestartCommandQueue();

    D3D12_VERTEX_BUFFER_VIEW bufferView = {};
    HRESULT hr;
    ID3D12Resource* pResource = NULL;
    ID3D12Resource* pUpload = NULL;

    hr = m_pDevice->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(vertexDataSize),
        D3D12_RESOURCE_STATE_GENERIC_READ,
        NULL,
        IID_PPV_ARGS(&pUpload));

    if (FAILED(hr))
        return new CDX12VertexBuffer(nullptr, nullptr, bufferView, 0);

    hr = m_pDevice->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(vertexDataSize),
        D3D12_RESOURCE_STATE_COPY_DEST,
        NULL,
        IID_PPV_ARGS(&pResource));

    if (FAILED(hr))
        return new CDX12VertexBuffer(nullptr, nullptr, bufferView, 0);

    D3D12_SUBRESOURCE_DATA data;
    data.pData = vertexData;
    data.RowPitch = vertexDataSize;
    data.SlicePitch = data.RowPitch;

    UpdateSubresources(m_pCommandList, pResource, pUpload, (UINT64)0, (UINT64)0, (UINT64)1, &data);

    m_pCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(pResource, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));

    m_pCommandList->Close();
    ID3D12CommandList* ppCommandLists[] = { m_pCommandList };
    m_pCommandQueue->ExecuteCommandLists(1, ppCommandLists);

    // increment the fence value now, otherwise the buffer might not be uploaded by the time we start drawing
    m_pFenceValues[nFrameIndex]++;
    hr = m_pCommandQueue->Signal(m_pFences[nFrameIndex], m_pFenceValues[nFrameIndex]);
    
    bufferView.BufferLocation = pResource->GetGPUVirtualAddress();
    bufferView.SizeInBytes = (UINT)vertexDataSize;
    bufferView.StrideInBytes = sizeof(MGenericVertex);

    return new CDX12VertexBuffer(pResource, pUpload, bufferView);
}

IIndexBuffer* CDX12Renderer::CreateIndexBuffer(void* indexData, size_t indexDataSize, EBufferUsage bufferUsage)
{
    WaitForPreviousFrame();
    RestartCommandQueue();

    size_t nNumInds = indexDataSize / sizeof(MGenericVertex);
    D3D12_INDEX_BUFFER_VIEW bufferView = {};
    HRESULT hr;
    ID3D12Resource* pResource = NULL;
    ID3D12Resource* pUpload = NULL;

    hr = m_pDevice->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(indexDataSize),
        D3D12_RESOURCE_STATE_GENERIC_READ,
        NULL,
        IID_PPV_ARGS(&pUpload));

    if (FAILED(hr))
        return new CDX12IndexBuffer(nullptr, nullptr, bufferView, 0, nNumInds);

    hr = m_pDevice->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(indexDataSize),
        D3D12_RESOURCE_STATE_COPY_DEST,
        NULL,
        IID_PPV_ARGS(&pResource));

    if (FAILED(hr))
        return new CDX12IndexBuffer(nullptr, nullptr, bufferView, 0, nNumInds);

    D3D12_SUBRESOURCE_DATA data;
    data.pData = indexData;
    data.RowPitch = indexDataSize;
    data.SlicePitch = data.RowPitch;

    UpdateSubresources(m_pCommandList, pResource, pUpload, (UINT64)0, (UINT64)0, (UINT64)1, &data);

    m_pCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(pResource, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER));

    m_pCommandList->Close();
    ID3D12CommandList* ppCommandLists[] = { m_pCommandList };
    m_pCommandQueue->ExecuteCommandLists(1, ppCommandLists);

    // increment the fence value now, otherwise the buffer might not be uploaded by the time we start drawing
    m_pFenceValues[nFrameIndex]++;
    hr = m_pCommandQueue->Signal(m_pFences[nFrameIndex], m_pFenceValues[nFrameIndex]);

    bufferView.BufferLocation = pResource->GetGPUVirtualAddress();
    bufferView.SizeInBytes = (UINT)indexDataSize;
    bufferView.Format = DXGI_FORMAT_R32_UINT;

    return new CDX12IndexBuffer(pResource, pUpload, bufferView, 0, nNumInds);
}

void CDX12Renderer::BindVertexBuffer(IVertexBuffer* pVertexBuffer) const
{
    CDX12VertexBuffer* pD3DVertexBuffer = reinterpret_cast<CDX12VertexBuffer*>(pVertexBuffer);
    D3D12_VERTEX_BUFFER_VIEW views[1] = { pD3DVertexBuffer->GetBufferView() };
    m_pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_pCommandList->IASetVertexBuffers(0, 1, views);
}

void CDX12Renderer::BindIndexBuffer(IIndexBuffer* pIndexBuffer) const
{
    CDX12IndexBuffer* pD3DIndexBuffer = reinterpret_cast<CDX12IndexBuffer*>(pIndexBuffer);
    D3D12_INDEX_BUFFER_VIEW views[1] = { pD3DIndexBuffer->GetBufferView() };
    m_pCommandList->IASetIndexBuffer(views);
}

IShader* CDX12Renderer::CreateVertexShader(const void* pShaderData, size_t nShaderDataSize)
{


    return nullptr;
}

IShader* CDX12Renderer::CreatePixelShader(const void* pShaderData, size_t nShaderDataSize)
{
    return nullptr;
}

IConstantBuffer* CDX12Renderer::CreateConstantBuffer(size_t nBufferSize)
{
    return nullptr;
}

void CDX12Renderer::DrawVertices(size_t numVerts)
{
    m_pCommandList->DrawInstanced((UINT)numVerts, 1, 0, 0);
}

void CDX12Renderer::DrawIndices(size_t numVerts)
{
}

void CDX12Renderer::SetViewport(MViewport viewport, MRect scissorView)
{
    D3D12_VIEWPORT D3Dviewport;
    D3Dviewport.Height = viewport.Height;
    D3Dviewport.MaxDepth = viewport.MaxDepth;
    D3Dviewport.MinDepth = viewport.MinDepth;
    D3Dviewport.TopLeftX = viewport.TopLeftX;
    D3Dviewport.TopLeftY = viewport.TopLeftY;
    D3Dviewport.Width = viewport.Width;

    D3D12_RECT D3DScissorRect;
    D3DScissorRect.left = scissorView.left;
    D3DScissorRect.right = scissorView.right;
    D3DScissorRect.bottom = scissorView.bottom;
    D3DScissorRect.top = scissorView.top;

    D3D12_VIEWPORT viewports[1] = { D3Dviewport };
    D3D12_RECT rects[1] = { D3DScissorRect };
    m_pCommandList->RSSetViewports(1, viewports);
    m_pCommandList->RSSetScissorRects(1, rects);
}

void CDX12Renderer::PresentBackBuffer()
{
    m_pSwapChain->Present(0, 0);
}

void CDX12Renderer::SwapBackBuffers()
{
    m_pCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_pRenderTargets[nFrameIndex], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
}

void CDX12Renderer::ClearRenderTarget(float color[4])
{
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_pRenderTargetView->GetCPUDescriptorHandleForHeapStart(), nFrameIndex, RenderTargetViewSize);
    m_pCommandList->ClearRenderTargetView(rtvHandle, color, 0, NULL);
}

void CDX12Renderer::BindCurrentBackBufferAsRenderTarget()
{
    m_pCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_pRenderTargets[nFrameIndex], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_pRenderTargetView->GetCPUDescriptorHandleForHeapStart(), nFrameIndex, RenderTargetViewSize);
    m_pCommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, NULL);
}

void CDX12Renderer::UpdateConstantBuffer(IConstantBuffer* pBuffer, void* pData)
{
}

void CDX12Renderer::BindPipelineStateObject(IPipelineState* pPipelineState)
{
    CDX12PipelineState* pD3DPipelineState = reinterpret_cast<CDX12PipelineState*>(pPipelineState);
    m_pMainPipelineState = pD3DPipelineState->GetPipelineState();
}

void CDX12Renderer::BindVertexShader(IShader* pShader) const
{
}

void CDX12Renderer::BindPixelShader(IShader* pShader) const
{
}

void CDX12Renderer::BindVertConstantBuffer(size_t nSlot, IConstantBuffer* pBuffer)
{
}

void CDX12Renderer::BindPixelConstantBuffer(size_t nSlot, IConstantBuffer* pBuffer)
{
}

void CDX12Renderer::SubmitMesh(IVertexBuffer* pVertex, IIndexBuffer* pIndex, IShader* pVertexShader, IShader* pPixelShader)
{
}

void CDX12Renderer::RestartCommandQueue()
{
    m_pCommandAllocator[nFrameIndex]->Reset();

    m_pCommandList->Reset(m_pCommandAllocator[nFrameIndex], m_pMainPipelineState);

    m_pCommandList->SetGraphicsRootSignature(m_pRootSignature);
}

void CDX12Renderer::ExecuteCommandQueue()
{
    ID3D12CommandList* ppCommandLists[] = { m_pCommandList };
    m_pCommandQueue->ExecuteCommandLists(1, ppCommandLists);
    m_pCommandQueue->Signal(m_pFences[nFrameIndex], m_pFenceValues[nFrameIndex]);
}

void CDX12Renderer::FinishCommandQueue()
{
    m_pCommandList->Close();
}

void CDX12Renderer::SubmitSceneBuffer(CSceneBuffer buffer)
{
}

void CDX12Renderer::WaitForPreviousFrame()
{
    HRESULT hr;

    // swap the current rtv buffer index so we draw on the correct buffer
    nFrameIndex = m_pSwapChain->GetCurrentBackBufferIndex();

    // if the current fence value is still less than "fenceValue", then we know the GPU has not finished executing
    // the command queue since it has not reached the "commandQueue->Signal(fence, fenceValue)" command
    if (m_pFences[nFrameIndex]->GetCompletedValue() < m_pFenceValues[nFrameIndex])
    {
        // we have the fence create an event which is signaled once the fence's current value is "fenceValue"
        hr = m_pFences[nFrameIndex]->SetEventOnCompletion(m_pFenceValues[nFrameIndex], m_FenceEvent);

        // We will wait until the fence has triggered the event that it's current value has reached "fenceValue". once it's value
        // has reached "fenceValue", we know the command queue has finished executing
        WaitForSingleObject(m_FenceEvent, INFINITE);
    }

    // increment fenceValue for next frame
    m_pFenceValues[nFrameIndex]++;
}

void CDX12Renderer::SetInitialVertexShaderBytecode(const char* pData, size_t size)
{
    m_pInitialVertexShader.BytecodeLength = size;
    m_pInitialVertexShader.pShaderBytecode = pData;
}

// -- Vertex Shader -- //

CDX12VertexBuffer::CDX12VertexBuffer(ID3D12Resource* pResource, ID3D12Resource* pUploadBuffer, D3D12_VERTEX_BUFFER_VIEW bufferView, uint64_t nId)
{
    m_pVertexBuffer = pResource;
    m_pUploadBuffer = pUploadBuffer;
    m_BufferView = bufferView;
    m_nIdFromAssetId = nId;
}

CDX12VertexBuffer::~CDX12VertexBuffer()
{
    if (m_pUploadBuffer)
        m_pUploadBuffer->Release();
    if (m_pVertexBuffer)
        m_pVertexBuffer->Release();
}

void CDX12VertexBuffer::ReleaseBuffer()
{
    delete this;
}

CDX12PipelineState::CDX12PipelineState(ID3D12PipelineState* pPipelineState)
{
    m_pPipelineState = pPipelineState;
}

CDX12PipelineState::~CDX12PipelineState()
{
    m_pPipelineState->Release();
}

void CDX12PipelineState::ReleaseStateObject()
{
    delete this;
}

ID3D12PipelineState* CDX12PipelineState::GetPipelineState()
{
    return m_pPipelineState;
}

CDX12IndexBuffer::CDX12IndexBuffer(ID3D12Resource* pResource, ID3D12Resource* pUploadBuffer, D3D12_INDEX_BUFFER_VIEW bufferView, uint64_t nId, size_t nNumInds)
{
    m_pIndexBuffer = pResource;
    m_pUploadBuffer = pUploadBuffer;
    m_BufferView = bufferView;
    m_nIdFromAssetId = nId;
    m_nNumInds = nNumInds;
}

CDX12IndexBuffer::~CDX12IndexBuffer()
{
    if (m_pIndexBuffer)
        m_pIndexBuffer->Release();

    if (m_pUploadBuffer)
        m_pUploadBuffer->Release();
}

void CDX12IndexBuffer::ReleaseBuffer()
{
    delete this;
}
