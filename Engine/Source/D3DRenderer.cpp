#include "D3Drenderer.h"
#include "GameInfo.h"

void CD3DRenderer::InitRenderer(HWND hwnd)
{
    std::wstring wszShaderPath(MGameInfo::GetGameInfo().ShaderPath.begin(), MGameInfo::GetGameInfo().ShaderPath.end());
    ShaderPath = wszShaderPath.c_str();

    std::wstring wszShaderSourcePath(MGameInfo::GetGameInfo().RawShaderPath.begin(), MGameInfo::GetGameInfo().RawShaderPath.end());
    ShaderSourcePath = wszShaderSourcePath.c_str();

    DXGI_SWAP_CHAIN_DESC swap_chain_descr = { 0 };
    swap_chain_descr.BufferDesc.RefreshRate.Numerator = 0;
    swap_chain_descr.BufferDesc.RefreshRate.Denominator = 1;
    swap_chain_descr.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swap_chain_descr.SampleDesc.Count = 1;
    swap_chain_descr.SampleDesc.Quality = 0;
    swap_chain_descr.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swap_chain_descr.BufferCount = 1;
    swap_chain_descr.OutputWindow = hwnd;
    swap_chain_descr.Windowed = true;

    D3D_FEATURE_LEVEL feature_level;
    UINT flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#if defined( DEBUG ) || defined( _DEBUG )
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
        &swap_chain_descr,
        &m_pd3dSwapChain,
        &m_pd3dDevice,
        &feature_level,
        &m_pd3dDeviceContext);
    assert(S_OK == hr && m_pd3dSwapChain && m_pd3dDevice && m_pd3dDeviceContext);

    hr = m_pd3dSwapChain->GetBuffer(
        0,
        __uuidof(ID3D11Texture2D),
        (void**)&m_pd3dFrameBuffer);
    assert(SUCCEEDED(hr));

    hr = m_pd3dDevice->CreateRenderTargetView(
        m_pd3dFrameBuffer, 0, &m_pd3dRenderTargetView);
    assert(SUCCEEDED(hr));
    m_pd3dFrameBuffer->Release();

    CreateShaders();

    float vertex_data_array[] = {
   0.0f,  0.5f,  0.0f, // point at top
   0.5f, -0.5f,  0.0f, // point at bottom-right
  -0.5f, -0.5f,  0.0f, // point at bottom-left
    };

    D3D11_BUFFER_DESC vertex_buff_descr = {};
    vertex_buff_descr.ByteWidth = sizeof(vertex_data_array);
    vertex_buff_descr.Usage = D3D11_USAGE_DEFAULT;
    vertex_buff_descr.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    D3D11_SUBRESOURCE_DATA sr_data = { 0 };
    sr_data.pSysMem = vertex_data_array;
    hr = m_pd3dDevice->CreateBuffer(
        &vertex_buff_descr,
        &sr_data,
        &m_pd3dVertexBuffer);
    assert(SUCCEEDED(hr));
}

void CD3DRenderer::CreateShaders()
{
    ID3DBlob* pVsBlob = NULL, *pPsBlob = NULL;
    pVsBlob = CreateShaderFromFile(L"shader.hlsl", L"Vertex.cso", "vs_5_0", "vs_main");
    pPsBlob = CreateShaderFromFile(L"shader.hlsl", L"Pixel.cso","ps_5_0" , "ps_main");

   HRESULT hr =  m_pd3dDevice->CreateVertexShader(
       pVsBlob->GetBufferPointer(),
       pVsBlob->GetBufferSize(),
       NULL,
       &m_pd3dVertexShader);
   assert(SUCCEEDED(hr));


   hr =  m_pd3dDevice->CreatePixelShader(
       pPsBlob->GetBufferPointer(),
       pPsBlob->GetBufferSize(),
       NULL,
       &m_pd3dPixelShader);
   assert(SUCCEEDED(hr));


   D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
  { "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  /*
  { "COL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  { "NOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  { "TEX", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  */
   };

   hr = m_pd3dDevice->CreateInputLayout(
       inputElementDesc,
       ARRAYSIZE(inputElementDesc),
       pVsBlob->GetBufferPointer(),
       pVsBlob->GetBufferSize(),
       &m_pd3dInputLayout);
   assert(SUCCEEDED(hr));
}

ID3DBlob* CD3DRenderer::CreateShaderFromFile(LPCWSTR Filename, LPCWSTR CompiledFilename, LPCSTR Compiler, LPCSTR Entrypoint)
{
    UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    flags |= D3DCOMPILE_DEBUG; // add more debug output
#endif

    ID3DBlob *blob, *ErrorBlob;

    std::wstringstream FilePath;
    FilePath << ShaderPath << CompiledFilename;

    std::wstringstream SourceFilePath;
    SourceFilePath << ShaderSourcePath << Filename;

    //Try to get precompiled shader file
    HRESULT hr = D3DReadFileToBlob(FilePath.str().c_str(), &blob);
    if (FAILED(hr))
    {
        //Compile shader source file
        HRESULT hr = D3DCompileFromFile(
            SourceFilePath.str().c_str(),
            nullptr,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            Entrypoint,
            Compiler,
            flags,
            0,
            &blob,
            &ErrorBlob);
        if (FAILED(hr)) {
            if (ErrorBlob) {
                ErrorBlob->Release();
                OutputDebugStringA((char*)ErrorBlob->GetBufferPointer());
            }
            if (blob) { blob->Release(); }
            assert(false);
        }
        else
        {
            hr = D3DWriteBlobToFile(blob, FilePath.str().c_str(), TRUE);
            if (FAILED(hr))
            {
                assert(false);
            }
            else
            {
                return blob;
            }
        }
    }
    else
    {
        return blob;
    }
}

void CD3DRenderer::RenderFrame(HWND hwnd)
{
    float flBkColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    m_pd3dDeviceContext->ClearRenderTargetView(m_pd3dRenderTargetView, flBkColor);

    RECT winRect;
    GetClientRect(hwnd, &winRect);
    D3D11_VIEWPORT viewport = {
      0.0f,
      0.0f,
      (FLOAT)(winRect.right - winRect.left),
      (FLOAT)(winRect.bottom - winRect.top),
      0.0f,
      1.0f };
    m_pd3dDeviceContext->RSSetViewports(1, &viewport);

    m_pd3dDeviceContext->OMSetRenderTargets(1, &m_pd3dRenderTargetView, NULL);

    m_pd3dDeviceContext->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_pd3dDeviceContext->IASetInputLayout(m_pd3dInputLayout);
    m_pd3dDeviceContext->IASetVertexBuffers(
        0,
        1,
        &m_pd3dVertexBuffer,
        &stride,
        &vertex_offset);

    m_pd3dDeviceContext->VSSetShader(m_pd3dVertexShader, NULL, 0);
    m_pd3dDeviceContext->PSSetShader(m_pd3dPixelShader, NULL, 0);

    m_pd3dDeviceContext->Draw(vertex_count, 0);

    m_pd3dSwapChain->Present(1, 0);
}
