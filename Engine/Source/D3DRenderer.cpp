#include "D3Drenderer.h"

void CD3DRenderer::InitRenderer(HWND hwnd)
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hwnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = true;

    D3D_FEATURE_LEVEL feature_level;
    UINT flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#if defined( DEBUG )
    flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        flags,
        NULL,
        0,
        D3D11_SDK_VERSION,
        &sd,
        &m_pd3dSwapChain,
        &m_pd3dDevice,
        &feature_level,
        &m_pd3dDeviceContext);
    assert(S_OK == hr && m_pd3dSwapChain && m_pd3dDevice && m_pd3dDeviceContext);

    hr = m_pd3dSwapChain->GetBuffer(
        0,
        __uuidof(ID3D11Texture2D),
        (void**)&m_pFrameBuffer);
    assert(SUCCEEDED(hr));

    hr = m_pd3dDevice->CreateRenderTargetView(
        m_pFrameBuffer, 0, &m_pRenderTargetView);
    assert(SUCCEEDED(hr));
    m_pFrameBuffer->Release();
}

void CD3DRenderer::CreateShaders()
{
    
}

void CD3DRenderer::RenderFrame(HWND hwnd)
{
    
}
