#include "D3Drenderer.h"
#include "GameInfo.h"
#include "UserSettings.h"
#include "Input.h"

CD3DRenderer* CD3DRenderer::g_pd3dRenderer = new CD3DRenderer();

void CD3DRenderer::InitRenderer(HWND hwnd)
{
    std::wstring wszShaderPath(MGameInfo::GetGameInfo().ShaderPath.begin(), MGameInfo::GetGameInfo().ShaderPath.end());
    ShaderPath = wszShaderPath.c_str();

    std::wstring wszShaderSourcePath(MGameInfo::GetGameInfo().RawShaderPath.begin(), MGameInfo::GetGameInfo().RawShaderPath.end());
    ShaderSourcePath = wszShaderSourcePath.c_str();

    int width = MUserSettings::GetUserSettings().nVSWidth;
    int height = MUserSettings::GetUserSettings().nVSWidth;


    DXGI_SWAP_CHAIN_DESC swap_chain_descr = { 0 };
    swap_chain_descr.BufferDesc.RefreshRate.Numerator = 0;
    swap_chain_descr.BufferDesc.RefreshRate.Denominator = 1;
    swap_chain_descr.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swap_chain_descr.SampleDesc.Count = 1;
    swap_chain_descr.SampleDesc.Quality = 0;
    swap_chain_descr.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swap_chain_descr.BufferCount = 1;
    swap_chain_descr.OutputWindow = hwnd;
    swap_chain_descr.Windowed = MUserSettings::GetUserSettings().bVSWindowed;

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

    
}

void CD3DRenderer::CreateShaders()
{
    
}

ID3DBlob* CD3DRenderer::CreateShaderFromFile(LPCWSTR Filename, LPCWSTR CompiledFilename, LPCSTR Compiler, LPCSTR Entrypoint)
{
    UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    flags |= D3DCOMPILE_DEBUG; // add more debug output
#endif

    ID3DBlob* blob, * ErrorBlob;

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
    return blob;
}

void CD3DRenderer::RenderFrame(HWND hwnd)
{
    
}