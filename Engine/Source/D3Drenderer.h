#pragma once
#include "Renderer.h"
#include "imgui.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <d3dcompiler.h>

#pragma comment( lib, "user32" )          // link against the win32 library
#pragma comment( lib, "d3d11.lib" )       // direct3D library
#pragma comment( lib, "dxgi.lib" )        // directx graphics interface
#pragma comment( lib, "d3dcompiler.lib" ) // shader compiler

struct cbPerObject
{
	DirectX::XMMATRIX WVP;
};

class CD3DRenderer : public CRenderer
{
public:
	static CD3DRenderer* GetD3DRenderer() { return g_pd3dRenderer; }

	virtual void InitRenderer(HWND hwnd) override;

	virtual void CreateShaders() override;

	ID3DBlob* CreateShaderFromFile(LPCWSTR Filename, LPCWSTR CompiledFilename, LPCSTR Compiler, LPCSTR Entrypoint);

	virtual void RenderFrame(HWND hwnd) override;

private:
	static CD3DRenderer* g_pd3dRenderer;

	CD3DRenderer() {}
	~CD3DRenderer() {}

	ID3D11Device*				m_pd3dDevice = NULL;
	ID3D11DeviceContext*		m_pd3dDeviceContext = NULL;
	IDXGISwapChain*				m_pd3dSwapChain = NULL;

	const wchar_t* ShaderPath;
	const wchar_t* ShaderSourcePath;
	UINT stride = sizeof(MVertex);
	UINT vertex_offset = 0;
	UINT vertex_count = 3;
};