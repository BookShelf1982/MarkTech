#pragma once
#include "Renderer.h"
#include <assert.h>
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#include <d3d11sdklayers.h>
#include <DirectXMath.h>

#pragma comment( lib, "user32" )          // link against the win32 library
#pragma comment( lib, "d3d11.lib" )       // direct3D library
#pragma comment( lib, "dxgi.lib" )        // directx graphics interface
#pragma comment( lib, "d3dcompiler.lib" ) // shader compiler

#pragma warning(disable : 4251)

namespace MarkTech
{
	struct MConstBuffer
	{
		DirectX::XMMATRIX WVP;
	};


	class MARKTECH_API CD3D11Renderer : public CRenderer
	{
	public:
		virtual bool InitRenderer(const CWinWindow& window) override;

		virtual void RenderFrame(const CWinWindow& window) override;

		virtual void CreateShaders() override;

		void UpdateRender(const CWinWindow& window);

		ID3DBlob* GetShaderBytecodeFromFile(LPCWSTR Filename, LPCWSTR CompiledFilename, LPCSTR Compiler, LPCSTR Entrypoint);

		void UpdateCameraData(const MCameraData& data);

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
		ID3D11DepthStencilView*		m_pDepthStencilView = NULL;
		ID3D11Texture2D*			m_pDepthStencilBuffer = NULL;

		ID3D11Buffer*				m_pMainVertexBuffer = NULL;
		ID3D11Buffer*				m_pMainIndexBuffer = NULL;

		ID3D11VertexShader*			m_pVertexShader = NULL;
		ID3D11PixelShader*			m_pPixelShader = NULL;
		ID3D11Texture2D*			m_pTexture = NULL;
		ID3D11ShaderResourceView*	m_pTextureView = NULL;
		ID3D11SamplerState*			m_pTextureSampler = NULL;
		ID3D11Buffer*				m_pConstBuffer = NULL;
		ID3D11InputLayout*			m_pInputLayout = NULL;

		ID3D11Debug*				m_pDebug = NULL;

		UINT stride = (UINT)sizeof(MVertex);
		UINT offset = 0;

		char* szSourcePath;
		char* szCompiledPath;

		MConstBuffer constBuffer;

		//Math
		DirectX::XMMATRIX WVP;
		DirectX::XMMATRIX World;
		DirectX::XMMATRIX camView;
		DirectX::XMMATRIX camProjection;

		DirectX::XMVECTOR rcamPosition;
		DirectX::XMVECTOR rcamTarget;
		DirectX::XMVECTOR rcamUp;

		//Camera
		MCameraData camData;
		
	};
	static CD3D11Renderer* GetD3DRenderer() { return CD3D11Renderer::GetD3DRenderer(); }
}