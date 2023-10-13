#pragma once
#include "Renderer.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <d3dcompiler.h>

#pragma comment( lib, "user32" )          // link against the win32 library
#pragma comment( lib, "d3d11.lib" )       // direct3D library
#pragma comment( lib, "dxgi.lib" )        // directx graphics interface
#pragma comment( lib, "d3dcompiler.lib" ) // shader compiler

class CD3DRenderer : public CRenderer
{
public:
	virtual void InitRenderer(HWND hwnd) override;

	virtual void CreateShaders() override;

	ID3DBlob* CreateShaderFromFile(LPCWSTR Filename, LPCWSTR CompiledFilename, LPCSTR Compiler, LPCSTR Entrypoint);

	virtual void RenderFrame(HWND hwnd) override;

private:
	ID3D11Device* m_pd3dDevice = NULL;
	ID3D11DeviceContext* m_pd3dDeviceContext = NULL;
	IDXGISwapChain* m_pd3dSwapChain = NULL;
	ID3D11RenderTargetView* m_pd3dRenderTargetView = NULL;
	ID3D11Texture2D* m_pd3dFrameBuffer = NULL;
	ID3D11VertexShader* m_pd3dVertexShader = NULL;
	ID3D11PixelShader* m_pd3dPixelShader = NULL;
	ID3D11InputLayout* m_pd3dInputLayout = NULL;
	ID3D11Buffer* m_pd3dVertexBuffer = NULL;

	const wchar_t* ShaderPath;
	const wchar_t* ShaderSourcePath;
	UINT stride = 3 * sizeof(float);
	UINT vertex_offset = 0;
	UINT vertex_count = 3;
};