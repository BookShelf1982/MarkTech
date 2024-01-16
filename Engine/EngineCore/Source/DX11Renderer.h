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
	virtual IVertexBuffer* CreateVertexBuffer(void* vertexData, size_t vertexDataSize, EBufferUsage bufferUsage) override;
	virtual IIndexBuffer* CreateIndexBuffer(void* indexData, size_t indexDataSize, EBufferUsage bufferUsage) override;
	virtual void BindVertexBuffer(IVertexBuffer* pVertexBuffer) const override;
	virtual void BindIndexBuffer(IIndexBuffer* pIndexBuffer) const override;
	virtual IShader* CreateVertexShader(const void* pShaderData, size_t nShaderDataSize) override;
	virtual IShader* CreatePixelShader(const void* pShaderData, size_t nShaderDataSize) override;
	virtual IConstantBuffer* CreateConstantBuffer(size_t nBufferSize) override;
	virtual void UpdateConstantBuffer(IConstantBuffer* pBuffer, void* pData) override;
	virtual void BindVertexShader(IShader* pShader) const override;
	virtual void BindPixelShader(IShader* pShader) const override;
	virtual void BindVertConstantBuffer(size_t nSlot, IConstantBuffer* pBuffer) override;
	virtual void BindPixelConstantBuffer(size_t nSlot, IConstantBuffer* pBuffer) override;
	virtual void SubmitMesh(IVertexBuffer* pVertex, IIndexBuffer* pIndex, IShader* pVertexShader, IShader* pPixelShader) override;
	virtual void FinishCommandQueue() override;
	virtual void SubmitRenderBuffer(CRenderBuffer pBuffer) override;
private:
	CWinWindow*				m_pWindowRef = NULL;

	IDXGIFactory*			m_pDXGIFactory = NULL;
	ID3D11Device*			m_pd3dDevice = NULL;
	ID3D11DeviceContext*	m_pd3dDeviceContext = NULL;
	ID3D11DeviceContext*	m_pd3dDeferredDeviceContext = NULL;
	ID3D11CommandList*		m_pd3dCommandList = NULL;
	ID3D11InputLayout*		m_pd3dInputLayout = NULL;

	IDXGISwapChain*			m_pd3dSwapChain = NULL;
	ID3D11RenderTargetView* m_pd3dRenderTargetView = NULL;
	ID3D11Texture2D*		m_pd3dBackBuffer = NULL;
	ID3D11DepthStencilView* m_pd3dDepthStencilView = NULL;
	ID3D11Texture2D*		m_pd3dDepthStencilBuffer = NULL;
	ID3D11BlendState*		m_pTransparency = NULL;
	D3D11_VIEWPORT			m_pd3dViewport;

	IVertexBuffer*			m_pMainVertexBuffer = NULL;
	IIndexBuffer*			m_pMainIndexBuffer = NULL;

	IShader*				m_pVertexShader = NULL;
	IShader*				m_pPixelShader = NULL;

	IConstantBuffer*		m_pObjectCBuffer = NULL;
	IConstantBuffer*		m_pWorldCBuffer = NULL;
	IConstantBuffer*		m_pLambertCBuffer = NULL;
	MObjectConstBuffer		m_ObjectCBufferData;
	MWorldConstBuffer		m_WorldCBufferData;
	MLambertProps			mLambertProps;


	float color[4] = { 0.0f, 0.0f, 0.55f, 1.0f };
	float objectColor[4] = { 0.0f, 0.0f, 0.55f, 1.0f };
	float objectPos[3] = { 3.0f, 0.0f, 0.0f };
	float objectRot[3] = { 0.0f, 0.0f, 0.0f };
	float sunPos[4] = { 10.0f, 0.0f, 10.0f, 1.0f };

#ifdef DEBUG
	ID3D11Debug* m_pd3dDebug;
#endif // DEBUG


};

bool CreateDX11Renderer(IRenderer** ppRenderer);

class CDX11VertexBuffer : public IVertexBuffer
{
public:
	CDX11VertexBuffer(ID3D11Buffer* pVertexBuffer, size_t nVerts);
	~CDX11VertexBuffer();
	virtual void ReleaseBuffer() override;
	inline ID3D11Buffer* GetBuffer() const { return m_pVertexBuffer; }
private:
	ID3D11Buffer* m_pVertexBuffer;
	size_t m_nVerts;
};

class CDX11IndexBuffer : public IIndexBuffer
{
public:
	CDX11IndexBuffer(ID3D11Buffer* pIndexBuffer, size_t nInds);
	~CDX11IndexBuffer();
	virtual void ReleaseBuffer() override;
	inline ID3D11Buffer* GetBuffer() const { return m_pIndexBuffer; }
	virtual inline size_t GetInds() const override { return m_nInds; }
private:
	ID3D11Buffer* m_pIndexBuffer;
	size_t m_nInds;
};

class CDX11VertexShader : public IShader
{
public:
	CDX11VertexShader(ID3D11VertexShader* pShader);
	~CDX11VertexShader();
	virtual void ReleaseShader() override;
	inline ID3D11VertexShader* GetShader() const { return m_pVertexShader; }
	inline EShaderType GetType()  const { return m_Type; }
private:
	EShaderType m_Type;
	ID3D11VertexShader* m_pVertexShader;
};

class CDX11PixelShader : public IShader
{
public:
	CDX11PixelShader(ID3D11PixelShader* pShader);
	~CDX11PixelShader();
	virtual void ReleaseShader() override;
	inline ID3D11PixelShader* GetShader() const { return m_pPixelShader; }
	inline EShaderType GetType()  const { return m_Type; }
private:
	EShaderType m_Type;
	ID3D11PixelShader* m_pPixelShader;
};

class CDX11ConstantBuffer : public IConstantBuffer
{
public:
	CDX11ConstantBuffer(ID3D11Buffer* pBuffer);
	~CDX11ConstantBuffer();
	virtual void ReleaseBuffer() override;
	inline ID3D11Buffer* GetBuffer() const { return m_pBuffer; }
private:
	ID3D11Buffer* m_pBuffer;
};