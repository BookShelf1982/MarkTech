#include "DX11Renderer.h"

void CDX11Renderer::SetWindow(IWindow* pWindow)
{
    CWinWindow* temp = dynamic_cast<CWinWindow*>(pWindow);
    if (temp)
    {
        m_pWindowRef = temp;
    }
}

bool CDX11Renderer::InitRenderer()
{
    m_API = ERendererAPI::Direct3D11;

    HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&m_pDXGIFactory);
    if (FAILED(hr))
        return false;

    D3D_FEATURE_LEVEL features[6] = {
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
                    D3D11_CREATE_DEVICE_SINGLETHREADED|D3D11_CREATE_DEVICE_DEBUG,
                    features,
                    6,
                    D3D11_SDK_VERSION,
                    &m_pd3dDevice,
                    &currentfeature,
                    &m_pd3dDeviceContext);

    if (FAILED(hr))
        return false;

    DXGI_SWAP_CHAIN_DESC swapchainDesc;
    ZeroMemory(&swapchainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
    swapchainDesc.BufferDesc.Width = m_pWindowRef->GetWidth();
    swapchainDesc.BufferDesc.Height = m_pWindowRef->GetHeight();
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

    D3D11_TEXTURE2D_DESC depthBufferDesc;
    ZeroMemory(&depthBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));
    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.Width = m_pWindowRef->GetWidth();
    depthBufferDesc.Height = m_pWindowRef->GetHeight();
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.SampleDesc.Count = 1;
    depthBufferDesc.SampleDesc.Quality = 0;
    depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    hr = m_pd3dDevice->CreateTexture2D(&depthBufferDesc, NULL, &m_pd3dDepthStencilBuffer);
    hr = m_pd3dDevice->CreateDepthStencilView(m_pd3dDepthStencilBuffer, NULL, &m_pd3dDepthStencilView);
    m_pd3dDepthStencilBuffer->Release();

    m_pd3dViewport.TopLeftX = (FLOAT)m_pWindowRef->GetXPos();
    m_pd3dViewport.TopLeftY = (FLOAT)m_pWindowRef->GetYPos();
    m_pd3dViewport.Width = (FLOAT)m_pWindowRef->GetWidth();
    m_pd3dViewport.Height = (FLOAT)m_pWindowRef->GetHeight();
    m_pd3dViewport.MaxDepth = 1.0f;
    m_pd3dViewport.MinDepth = 0.0f;



    return true;
}

void CDX11Renderer::RenderFrame()
{
    m_pd3dDeviceContext->OMSetRenderTargets(1, &m_pd3dRenderTargetView, m_pd3dDepthStencilView);
    float color[4] = { 0.0f, 0.0f, 0.55f, 1.0f };
    m_pd3dDeviceContext->ClearRenderTargetView(m_pd3dRenderTargetView, color);
    m_pd3dDeviceContext->ClearDepthStencilView(m_pd3dDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    m_pd3dDeviceContext->RSSetViewports(1, &m_pd3dViewport);

    m_pd3dSwapChain->Present(0, 0);
}

void CDX11Renderer::ShutdownRenderer()
{
    if (m_pDXGIFactory)
        m_pDXGIFactory->Release();
    if(m_pd3dDevice)
        m_pd3dDevice->Release();
    if(m_pd3dDeviceContext)
        m_pd3dDeviceContext->Release();
    if(m_pd3dSwapChain)
        m_pd3dSwapChain->Release();
    if (m_pd3dDepthStencilView)
        m_pd3dDepthStencilView->Release();
    if (m_pd3dRenderTargetView)
        m_pd3dRenderTargetView->Release();
}

IVertexBuffer* CDX11Renderer::CreateVertexBuffer(MGenericVertex* vertexData, size_t vertexDataSize)
{
    ID3D11Buffer* vertBuffer = NULL;

    D3D11_BUFFER_DESC vDesc = {};
    vDesc.Usage = D3D11_USAGE_DEFAULT;
    vDesc.ByteWidth = sizeof(MGenericVertex) * (UINT)vertexDataSize;
    vDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vDesc.CPUAccessFlags = 0;
    vDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA vData = {};
    vData.pSysMem = vertexData;

    m_pd3dDevice->CreateBuffer(&vDesc, &vData, &vertBuffer);

    return new CDX11VertexBuffer(vertBuffer);
}

void CDX11Renderer::BindVertexBuffer(IVertexBuffer* pVertexBuffer) const
{
    CDX11VertexBuffer* v = dynamic_cast<CDX11VertexBuffer*>(pVertexBuffer);
    if (v)
    {
        ID3D11Buffer* buffer = v->GetBuffer();
        m_pd3dDeviceContext->IASetVertexBuffers(0, 1, &buffer, &m_nVertexOffset, &m_nVertexOffset);
    }
}

bool CreateDX11Renderer(IRenderer** pRenderer)
{
    *pRenderer = new CDX11Renderer();
    return true;
}

CDX11VertexBuffer::CDX11VertexBuffer(ID3D11Buffer* pVertexBuffer)
    :m_pVertexBuffer(pVertexBuffer)
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