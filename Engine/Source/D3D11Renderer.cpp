#include "D3D11Renderer.h"

namespace MarkTech
{
	namespace Renderer
	{
		CD3D11Renderer* CD3D11Renderer::g_pd3dRenderer = new CD3D11Renderer();

		bool CD3D11Renderer::InitRenderer(HWND hwnd)
		{
			DXGI_SWAP_CHAIN_DESC SCDesc;
			ZeroMemory(&SCDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
			SCDesc.BufferDesc.RefreshRate.Numerator = 0;
			SCDesc.BufferDesc.RefreshRate.Denominator = 1;
			SCDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			SCDesc.SampleDesc.Count = 1;
			SCDesc.SampleDesc.Quality = 0;
			SCDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			SCDesc.BufferCount = 1;
			SCDesc.OutputWindow = hwnd;
			SCDesc.Windowed = true;


			D3D_FEATURE_LEVEL feature_level;
			UINT flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
		#if defined( DEBUG )
			flags |= D3D11_CREATE_DEVICE_DEBUG;
		#endif

			HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flags, NULL, 0, D3D11_SDK_VERSION, &SCDesc, &m_pSwapChain, &m_pd3dDevice, &feature_level, &m_pd3dDeviceContext);
			assert(S_OK == hr && m_pSwapChain && m_pd3dDevice && m_pd3dDeviceContext);

			hr = m_pd3dDevice->QueryInterface(__uuidof(ID3D11Debug), (void**)&m_pDebug);
			assert(SUCCEEDED(hr));
			
			hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_pBackBuffer);
			assert(SUCCEEDED(hr));

			hr = m_pd3dDevice->CreateRenderTargetView(m_pBackBuffer, NULL, &m_pMainRenderTargetView);
			assert(SUCCEEDED(hr));

			//m_pDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);

			m_pBackBuffer->Release();
			
			return true;
		}

		void CD3D11Renderer::RenderFrame(HWND hwnd)
		{
			float color[4] = { 0.0f,0.0f,0.0f,1.0f };

			m_pd3dDeviceContext->ClearRenderTargetView(m_pMainRenderTargetView, color);
			
			RECT WinRect;
			GetClientRect(hwnd, &WinRect);
			D3D11_VIEWPORT viewport;
			viewport.TopLeftX = 0;
			viewport.TopLeftY = 0;
			viewport.Width = (FLOAT)(WinRect.right - WinRect.left);
			viewport.Height = (FLOAT)(WinRect.bottom - WinRect.top);
			viewport.MaxDepth = 1.0f;
			viewport.MinDepth = 0.0f;
			m_pd3dDeviceContext->RSSetViewports(1, &viewport);

			m_pSwapChain->Present(1, 0);
		}

		void CD3D11Renderer::DestroyRenderer()
		{
			//m_pMainRenderTargetView->Release();
			m_pSwapChain->Release();
			m_pd3dDevice->Release();
			m_pd3dDeviceContext->Release();
			m_pDebug->Release();
			m_pMainRenderTargetView->Release();

			delete g_pd3dRenderer;
		}

	}
}