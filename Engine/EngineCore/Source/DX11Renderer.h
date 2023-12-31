#pragma once
#include "Renderer.h"
#include "WinWindow.h"
#include <d3d11.h>
#pragma comment(lib , "d3d11.lib")
#pragma comment( lib, "dxgi.lib" ) 

class CDX11Renderer : public IRenderer
{
public:
	virtual void SetWindow(IWindow* pWindow) override;
	virtual	bool InitRenderer() override;
	virtual	void RenderFrame() override;
	virtual	void ShutdownRenderer() override;
	virtual IVertexBuffer* CreateVertexBuffer(MGenericVertex* vertexData, size_t vertexDataSize) override;
	virtual void BindVertexBuffer(IVertexBuffer* pVertexBuffer) const override;
private:
	CWinWindow*				m_pWindowRef = NULL;

	IDXGIFactory*			m_pDXGIFactory = NULL;
	ID3D11Device*			m_pd3dDevice = NULL;
	ID3D11DeviceContext*	m_pd3dDeviceContext = NULL;
	IDXGISwapChain*			m_pd3dSwapChain = NULL;
	ID3D11RenderTargetView* m_pd3dRenderTargetView = NULL;
	ID3D11Texture2D*		m_pd3dBackBuffer = NULL;
	ID3D11DepthStencilView* m_pd3dDepthStencilView = NULL;
	ID3D11Texture2D*		m_pd3dDepthStencilBuffer = NULL;
	D3D11_VIEWPORT			m_pd3dViewport;
};

bool CreateDX11Renderer(IRenderer** ppRenderer);

class CDX11VertexBuffer : public IVertexBuffer
{
public:
	CDX11VertexBuffer(ID3D11Buffer* pVertexBuffer);
	~CDX11VertexBuffer();
	virtual void ReleaseBuffer() override;
	inline ID3D11Buffer* GetBuffer() const { return m_pVertexBuffer; }
private:
	ID3D11Buffer* m_pVertexBuffer;
};