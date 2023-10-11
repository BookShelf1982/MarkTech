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

	virtual void RenderFrame(HWND hwnd) override;

private:
	ID3D11Device* m_pd3dDevice;
	ID3D11DeviceContext* m_pd3dDeviceContext;
	IDXGISwapChain* m_pd3dSwapChain;
	ID3D11Texture2D* m_pFrameBuffer;
	ID3D11RenderTargetView* m_pRenderTargetView;
};