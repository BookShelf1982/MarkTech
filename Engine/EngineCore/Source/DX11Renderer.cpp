#include "DX11Renderer.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"


void CDX11Renderer::SetWindow(IWindow* pWindow)
{
    CWinWindow* temp = dynamic_cast<CWinWindow*>(pWindow);
    if (temp)
    {
        m_pWindowRef = temp;
    }
}

void CDX11Renderer::SetLevel(CLevel* pLevel)
{
    m_pLevelRef = pLevel;
}

bool CDX11Renderer::InitRenderer()
{
    m_API = ERendererAPI::Direct3D11;

    HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&m_pDXGIFactory);
    if (FAILED(hr))
        return false;

    D3D_FEATURE_LEVEL features[7] = {
                      D3D_FEATURE_LEVEL_11_1,
                      D3D_FEATURE_LEVEL_11_0,
                      D3D_FEATURE_LEVEL_10_1,
                      D3D_FEATURE_LEVEL_10_0,
                      D3D_FEATURE_LEVEL_9_3,
                      D3D_FEATURE_LEVEL_9_2,
                      D3D_FEATURE_LEVEL_9_1};

    D3D_FEATURE_LEVEL currentfeature;
    hr = D3D11CreateDevice(
                    NULL,
                    D3D_DRIVER_TYPE_HARDWARE,
                    NULL,
                    D3D11_CREATE_DEVICE_DEBUG,
                    features,
                    7,
                    D3D11_SDK_VERSION,
                    &m_pd3dDevice,
                    &currentfeature,
                    &m_pd3dDeviceContext);

    if (FAILED(hr))
        return false;

#ifdef DEBUG
    
#endif // DEBUG

    hr = m_pd3dDevice->CreateDeferredContext(0, &m_pd3dDeferredDeviceContext);

    if (FAILED(hr))
        return false;

    DXGI_SWAP_CHAIN_DESC swapchainDesc;
    ZeroMemory(&swapchainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
    swapchainDesc.BufferDesc.Width = 0;
    swapchainDesc.BufferDesc.Height = 0;
    swapchainDesc.BufferDesc.RefreshRate.Numerator = 0;
    swapchainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapchainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swapchainDesc.BufferCount = 2;
    swapchainDesc.SampleDesc.Count = 1;
    swapchainDesc.SampleDesc.Quality = 0;
    swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapchainDesc.OutputWindow = m_pWindowRef->GetHWND();
    swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapchainDesc.Windowed = true;

    hr = m_pDXGIFactory->CreateSwapChain(m_pd3dDevice, &swapchainDesc, &m_pd3dSwapChain);
    if (FAILED(hr))
        return false;

    hr = m_pd3dSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_pd3dBackBuffer);

    hr = m_pd3dDevice->CreateRenderTargetView(m_pd3dBackBuffer, NULL, &m_pd3dRenderTargetView);

    m_pd3dBackBuffer->Release();

    RECT winRect;
    GetClientRect(m_pWindowRef->GetHWND(), &winRect);

    D3D11_TEXTURE2D_DESC depthBufferDesc;
    ZeroMemory(&depthBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));
    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.Width = winRect.right - winRect.left;
    depthBufferDesc.Height = winRect.bottom - winRect.top;
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.SampleDesc.Count = 1;
    depthBufferDesc.SampleDesc.Quality = 0;
    depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    hr = m_pd3dDevice->CreateTexture2D(&depthBufferDesc, NULL, &m_pd3dDepthStencilBuffer);
    hr = m_pd3dDevice->CreateDepthStencilView(m_pd3dDepthStencilBuffer, NULL, &m_pd3dDepthStencilView);
    m_pd3dDepthStencilBuffer->Release();

    D3D11_BLEND_DESC blendDesc;
    ZeroMemory(&blendDesc, sizeof(blendDesc));

    D3D11_RENDER_TARGET_BLEND_DESC rtbd;
    ZeroMemory(&rtbd, sizeof(rtbd));

    rtbd.BlendEnable = true;
    rtbd.SrcBlend = D3D11_BLEND_SRC_ALPHA;
    rtbd.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    rtbd.BlendOp = D3D11_BLEND_OP_ADD;
    rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
    rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
    rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
    rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    blendDesc.AlphaToCoverageEnable = false;
    blendDesc.RenderTarget[0] = rtbd;

    hr = m_pd3dDevice->CreateBlendState(&blendDesc, &m_pTransparency);

    if (FAILED(hr))
        return false;

    m_pd3dViewport.TopLeftX = (FLOAT)0;
    m_pd3dViewport.TopLeftY = (FLOAT)0;
    m_pd3dViewport.Width = (FLOAT)winRect.right - winRect .left;
    m_pd3dViewport.Height = (FLOAT)winRect.bottom - winRect.top;
    m_pd3dViewport.MaxDepth = 1.0f;
    m_pd3dViewport.MinDepth = 0.0f;

    uint64_t vershaderid = m_pLevelRef->LoadShaderAsset("Bin/Shaders/VertexShader.mfx");
    MShaderAsset* pShader = m_pLevelRef->GetShaderAsset(vershaderid);
    m_pVertexShader = CreateVertexShader(pShader->m_pShaderBytecode, pShader->m_nShaderBytecodeSize);

    uint64_t pixshaderid = m_pLevelRef->LoadShaderAsset("Bin/Shaders/PixelShader.mfx");
    MShaderAsset* pPixelShader = m_pLevelRef->GetShaderAsset(pixshaderid);
    m_pPixelShader = CreatePixelShader(pPixelShader->m_pShaderBytecode, pPixelShader->m_nShaderBytecodeSize);

    D3D11_INPUT_ELEMENT_DESC inDesc[3] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    hr = m_pd3dDevice->CreateInputLayout(inDesc, 3, pShader->m_pShaderBytecode, pShader->m_nShaderBytecodeSize, &m_pd3dInputLayout);

    if (FAILED(hr))
        return false;

    uint64_t id = m_pLevelRef->LoadModelAsset("Content/teapot.mmdl");
    MModelAsset* pModelRef = m_pLevelRef->GetModelAsset(id);
    m_pMainVertexBuffer = CreateVertexBuffer(pModelRef->m_pVertData, pModelRef->m_nNumVerts, EBufferUsage::Static);
    m_pMainIndexBuffer = CreateIndexBuffer(pModelRef->m_pIndData, pModelRef->m_nNumInds, EBufferUsage::Static);
    m_pObjectCBuffer = CreateConstantBuffer(sizeof(MObjectConstBuffer));
    m_pWorldCBuffer = CreateConstantBuffer(sizeof(MWorldConstBuffer));

    ImGui_ImplDX11_Init(m_pd3dDevice, m_pd3dDeviceContext);

    return true;
}

void CDX11Renderer::RenderFrame()
{
    m_pd3dDeferredDeviceContext->OMSetRenderTargets(1, &m_pd3dRenderTargetView, m_pd3dDepthStencilView);
    m_pd3dDeferredDeviceContext->OMSetBlendState(m_pTransparency, NULL, 0xFFFFFFFF);
    m_pd3dDeferredDeviceContext->ClearRenderTargetView(m_pd3dRenderTargetView, color);
    m_pd3dDeferredDeviceContext->ClearDepthStencilView(m_pd3dDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    m_pd3dDeferredDeviceContext->RSSetViewports(1, &m_pd3dViewport);
    m_pd3dDeferredDeviceContext->IASetInputLayout(m_pd3dInputLayout);
    m_pd3dDeferredDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    DirectX::XMMATRIX LookTo = DirectX::XMMatrixLookAtLH(
            DirectX::XMVectorSet(0.0f, 0.0f, -0.5f, 1.0f),
            DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
            DirectX::XMVectorSet(0.0f, 1.0f, 1.0f, 1.0f)
    );

    DirectX::XMMATRIX ViewProj = DirectX::XMMatrixPerspectiveFovLH(0.4f * 3.14f, (float)m_pWindowRef->GetWidth() / m_pWindowRef->GetHeight(), 1.0f, 1000.0f);

    m_ObjectCBufferData.WorldMatrix = DirectX::XMMatrixRotationRollPitchYaw(objectRot[1], objectRot[2], objectRot[0]) * DirectX::XMMatrixTranslation(objectPos[1], objectPos[2], objectPos[0]);
    DirectX::XMMATRIX WVP = m_ObjectCBufferData.WorldMatrix * LookTo * ViewProj;
    m_ObjectCBufferData.WorldViewPorjection = WVP;
    m_WorldCBufferData.SunPos.x = sunPos[1];
    m_WorldCBufferData.SunPos.y = sunPos[2];
    m_WorldCBufferData.SunPos.z = sunPos[0];
    m_WorldCBufferData.SunPos.w = 1.0f;
    UpdateConstantBuffer(m_pObjectCBuffer, &m_ObjectCBufferData);
    UpdateConstantBuffer(m_pWorldCBuffer, &m_WorldCBufferData);
    BindVertConstantBuffer(0, m_pObjectCBuffer);
    BindPixelConstantBuffer(0, m_pWorldCBuffer);
    SubmitMesh(m_pMainVertexBuffer, m_pMainIndexBuffer, m_pVertexShader, m_pPixelShader);
    FinishCommandQueue();
    if (m_pd3dCommandList)
        m_pd3dDeviceContext->ExecuteCommandList(m_pd3dCommandList, TRUE);
    
    m_pd3dDeviceContext->OMSetRenderTargets(1, &m_pd3dRenderTargetView, m_pd3dDepthStencilView);

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    //ImGui::ShowDemoWindow();

    ImGui::Begin("Debug");
    ImGui::ColorEdit4("Clear color", color);
    float min = 1.0f;
    float max = 30.0f;
    ImGui::DragFloat3("Position", objectPos, 0.1f);
    ImGui::DragFloat3("Rotation", objectRot, 0.1f);
    ImGui::DragFloat3("Sun Position", sunPos, 1.0f);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    
    // Update and Render additional Platform Windows
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }

    m_pd3dSwapChain->Present(0, 0);

    if (m_pd3dCommandList)
    {
        m_pd3dCommandList->Release();
        m_pd3dCommandList = nullptr;
    }
}

void CDX11Renderer::ShutdownRenderer()
{
    if (m_pDXGIFactory)
        m_pDXGIFactory->Release();
    if (m_pd3dDevice)
        m_pd3dDevice->Release();
    if (m_pd3dDeviceContext)
        m_pd3dDeviceContext->Release();
    if (m_pd3dDeferredDeviceContext)
        m_pd3dDeferredDeviceContext->Release();
    if (m_pd3dSwapChain)
        m_pd3dSwapChain->Release();
    if (m_pd3dInputLayout)
        m_pd3dInputLayout->Release();
    if (m_pd3dDepthStencilView)
        m_pd3dDepthStencilView->Release();
    if (m_pd3dRenderTargetView)
        m_pd3dRenderTargetView->Release();
    if (m_pd3dCommandList)
        m_pd3dCommandList->Release();
    if (m_pTransparency)
        m_pTransparency->Release();

    m_pMainIndexBuffer->ReleaseBuffer();
    m_pMainVertexBuffer->ReleaseBuffer();
    m_pVertexShader->ReleaseShader();
    m_pPixelShader->ReleaseShader();
    m_pObjectCBuffer->ReleaseBuffer();
    m_pWorldCBuffer->ReleaseBuffer();

    ImGui_ImplDX11_Shutdown();
    ImGui::DestroyContext();
}

IVertexBuffer* CDX11Renderer::CreateVertexBuffer(void* vertexData, size_t vertexDataSize, EBufferUsage bufferUsage)
{
    ID3D11Buffer* vertBuffer = NULL;

    D3D11_BUFFER_DESC vDesc = {};

    switch (bufferUsage)
    {
    case EBufferUsage::Static:
        vDesc.Usage = D3D11_USAGE_DEFAULT;
        vDesc.CPUAccessFlags = 0;
        break;
    case EBufferUsage::Dynamic:
        vDesc.Usage = D3D11_USAGE_DYNAMIC;
        vDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        break;
    }

    vDesc.ByteWidth = sizeof(MGenericVertex) * (UINT)vertexDataSize;
    vDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vDesc.MiscFlags = 0;

    if (vertexData == NULL)
    {
       HRESULT hr = m_pd3dDevice->CreateBuffer(&vDesc, NULL, &vertBuffer);
    }
    else
    {
        D3D11_SUBRESOURCE_DATA vData = {};
        vData.pSysMem = vertexData;

        HRESULT hr = m_pd3dDevice->CreateBuffer(&vDesc, &vData, &vertBuffer);
    }

    return new CDX11VertexBuffer(vertBuffer, vertexDataSize);
}

IIndexBuffer* CDX11Renderer::CreateIndexBuffer(void* indexData, size_t indexDataSize, EBufferUsage bufferUsage)
{
    ID3D11Buffer* indBuffer = NULL;

    D3D11_BUFFER_DESC iDesc = {};
    switch (bufferUsage)
    {
    case EBufferUsage::Static:
        iDesc.Usage = D3D11_USAGE_DEFAULT;
        iDesc.CPUAccessFlags = 0;
        break;
    case EBufferUsage::Dynamic:
        iDesc.Usage = D3D11_USAGE_DYNAMIC;
        iDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        break;
    }
    iDesc.ByteWidth = sizeof(uint32_t) * (UINT)indexDataSize;
    iDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    iDesc.MiscFlags = 0;

    if (indexData == NULL)
    {
        m_pd3dDevice->CreateBuffer(&iDesc, NULL, &indBuffer);
    }
    else
    {
        D3D11_SUBRESOURCE_DATA iData = {};
        iData.pSysMem = reinterpret_cast<MGenericVertex*>(indexData);

        m_pd3dDevice->CreateBuffer(&iDesc, &iData, &indBuffer);
    }

    return new CDX11IndexBuffer(indBuffer, indexDataSize);
}

void CDX11Renderer::BindVertexBuffer(IVertexBuffer* pVertexBuffer) const
{
    CDX11VertexBuffer* v = reinterpret_cast<CDX11VertexBuffer*>(pVertexBuffer);
    if (v)
    {
        ID3D11Buffer* buffer = v->GetBuffer();
        m_pd3dDeferredDeviceContext->IASetVertexBuffers(0, 1, &buffer, &m_nVertexStride, &m_nVertexOffset);
    }
}

void CDX11Renderer::BindIndexBuffer(IIndexBuffer* pIndexBuffer) const
{
    CDX11IndexBuffer* temp = reinterpret_cast<CDX11IndexBuffer*>(pIndexBuffer);
    if(temp)
        m_pd3dDeferredDeviceContext->IASetIndexBuffer(temp->GetBuffer(), DXGI_FORMAT_R32_UINT, 0);
}

IShader* CDX11Renderer::CreateVertexShader(const void* pShaderData, size_t nShaderDataSize)
{
    ID3D11VertexShader* pVertexShader;
    m_pd3dDevice->CreateVertexShader(pShaderData, nShaderDataSize, NULL, &pVertexShader);
    return new CDX11VertexShader(pVertexShader);
}

IShader* CDX11Renderer::CreatePixelShader(const void* pShaderData, size_t nShaderDataSize)
{
    ID3D11PixelShader* pPixelShader;
    m_pd3dDevice->CreatePixelShader(pShaderData, nShaderDataSize, NULL, &pPixelShader);
    return new CDX11PixelShader(pPixelShader);
}

IConstantBuffer* CDX11Renderer::CreateConstantBuffer(size_t nBufferSize)
{
    ID3D11Buffer* pBuffer;
    D3D11_BUFFER_DESC iDesc = {};
    iDesc.ByteWidth = (UINT)nBufferSize;
    iDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    iDesc.MiscFlags = 0;

    m_pd3dDevice->CreateBuffer(&iDesc, NULL, &pBuffer);

    return new CDX11ConstantBuffer(pBuffer);
}

void CDX11Renderer::UpdateConstantBuffer(IConstantBuffer* pBuffer, void* pData)
{
    CDX11ConstantBuffer* temp = reinterpret_cast<CDX11ConstantBuffer*>(pBuffer);
    m_pd3dDeferredDeviceContext->UpdateSubresource(temp->GetBuffer(), 0, NULL, pData, 0, 0);
}

void CDX11Renderer::BindVertexShader(IShader* pShader) const
{
    CDX11VertexShader* temp = reinterpret_cast<CDX11VertexShader*>(pShader);
    if (temp && temp->GetType() == EShaderType::Vertex) {
        m_pd3dDeferredDeviceContext->VSSetShader(temp->GetShader(), NULL, 0);
    }
}

void CDX11Renderer::BindPixelShader(IShader* pShader) const
{
    CDX11PixelShader* temp = reinterpret_cast<CDX11PixelShader*>(pShader);
    if (temp && temp->GetType() == EShaderType::Pixel) {
        m_pd3dDeferredDeviceContext->PSSetShader(temp->GetShader(), NULL, 0);
    }
}

void CDX11Renderer::BindVertConstantBuffer(size_t nSlot, IConstantBuffer* pBuffer)
{
    CDX11ConstantBuffer* temp = reinterpret_cast<CDX11ConstantBuffer*>(pBuffer);
    ID3D11Buffer* buff = temp->GetBuffer();
    m_pd3dDeferredDeviceContext->VSSetConstantBuffers((UINT)nSlot, 1, &buff);
}

void CDX11Renderer::BindPixelConstantBuffer(size_t nSlot, IConstantBuffer* pBuffer)
{
    CDX11ConstantBuffer* temp = reinterpret_cast<CDX11ConstantBuffer*>(pBuffer);
    ID3D11Buffer* buff = temp->GetBuffer();
    m_pd3dDeferredDeviceContext->PSSetConstantBuffers((UINT)nSlot, 1, &buff);
}

void CDX11Renderer::SubmitMesh(IVertexBuffer* pVertex, IIndexBuffer* pIndex, IShader* pVertexShader, IShader* pPixelShader)
{
    BindVertexBuffer(pVertex);
    BindIndexBuffer(pIndex);
    BindVertexShader(pVertexShader);
    BindPixelShader(pPixelShader);
    m_pd3dDeferredDeviceContext->DrawIndexed((UINT)pIndex->GetInds(), 0, 0);
}

void CDX11Renderer::FinishCommandQueue()
{
    m_pd3dDeferredDeviceContext->FinishCommandList(TRUE, &m_pd3dCommandList);
}

bool CreateDX11Renderer(IRenderer** pRenderer)
{
    *pRenderer = new CDX11Renderer();
    return true;
}

CDX11VertexBuffer::CDX11VertexBuffer(ID3D11Buffer* pVertexBuffer, size_t nVerts)
    :m_pVertexBuffer(pVertexBuffer), m_nVerts(nVerts)
{
}

CDX11VertexBuffer::~CDX11VertexBuffer()
{
    m_pVertexBuffer->Release();
}

void CDX11VertexBuffer::ReleaseBuffer()
{
    delete this;
}

CDX11IndexBuffer::CDX11IndexBuffer(ID3D11Buffer* pIndexBuffer, size_t nInds)
    :m_pIndexBuffer(pIndexBuffer), m_nInds(nInds)
{
}

CDX11IndexBuffer::~CDX11IndexBuffer()
{
    m_pIndexBuffer->Release();
}

void CDX11IndexBuffer::ReleaseBuffer()
{
    delete this;
}

CDX11VertexShader::CDX11VertexShader(ID3D11VertexShader* pShader)
    :m_pVertexShader(pShader), m_Type(EShaderType::Vertex)
{
}

CDX11VertexShader::~CDX11VertexShader()
{
    m_pVertexShader->Release();
}

void CDX11VertexShader::ReleaseShader()
{
    delete this;
}

CDX11PixelShader::CDX11PixelShader(ID3D11PixelShader* pShader)
    :m_pPixelShader(pShader), m_Type(EShaderType::Pixel)
{
}

CDX11PixelShader::~CDX11PixelShader()
{
    m_pPixelShader->Release();
}

void CDX11PixelShader::ReleaseShader()
{
    delete this;
}

CDX11ConstantBuffer::CDX11ConstantBuffer(ID3D11Buffer* pBuffer)
{
    m_pBuffer = pBuffer;
}

CDX11ConstantBuffer::~CDX11ConstantBuffer()
{
    m_pBuffer->Release();
}

void CDX11ConstantBuffer::ReleaseBuffer()
{
    delete this;
}
