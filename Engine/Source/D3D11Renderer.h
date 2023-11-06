#pragma once
#include "Renderer.h"
#include <assert.h>
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#include <d3d11sdklayers.h>

#pragma comment( lib, "user32" )          // link against the win32 library
#pragma comment( lib, "d3d11.lib" )       // direct3D library
#pragma comment( lib, "dxgi.lib" )        // directx graphics interface
#pragma comment( lib, "d3dcompiler.lib" ) // shader compiler

namespace MarkTech
{
	namespace Renderer
	{
		class MARKTECH_API CD3D11Renderer : public CRenderer
		{
		public:
			virtual bool InitRenderer(HWND hwnd) override;

			virtual void RenderFrame(HWND hwnd) override;

			void DestroyRenderer();

			static CD3D11Renderer* GetD3DRenderer() { return g_pd3dRenderer; }
		private:
			CD3D11Renderer() {}
			~CD3D11Renderer() {}

			static CD3D11Renderer* g_pd3dRenderer;

			ID3D11Device*				m_pd3dDevice = NULL;
			ID3D11DeviceContext*		m_pd3dDeviceContext = NULL;
			IDXGISwapChain*				m_pSwapChain = NULL;
			ID3D11RenderTargetView*		m_pMainRenderTargetView = NULL;
			ID3D11Texture2D*			m_pBackBuffer = NULL;

			ID3D11Buffer*				m_pMainVertexBuffer = NULL;
			ID3D11Buffer*				m_pMainIndexBuffer = NULL;

			ID3D11Debug*				m_pDebug = NULL;

		};
	}
}