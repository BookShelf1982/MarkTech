#include "3DRenderer.h"
#include <fstream>

C3DRenderer::C3DRenderer()
	:m_pRenderInterface(nullptr)
{
}

C3DRenderer::~C3DRenderer()
{
	delete m_pRenderInterface;
}

void C3DRenderer::SetRenderAPI(ERendererAPI api)
{
	switch (api)
	{
	case ERendererAPI::None:
		break;
	case ERendererAPI::Direct3D11:
		break;
	case ERendererAPI::Direct3D12:
		CreateDX12Renderer(&m_pRenderInterface);
		break;
	default:
		break;
	}
}

bool C3DRenderer::Init(IWindow* pWindow, CAssetRegistry* pAssetRegistry)
{
	m_pWindowRef = pWindow;
	m_pAssetRegistryRef = pAssetRegistry;
	uint64_t nVShaderId = m_pAssetRegistryRef->LoadShaderAsset("Bin/Shaders/VertexShader.mfx");
	uint64_t nPShaderId = m_pAssetRegistryRef->LoadShaderAsset("Bin/Shaders/PixelShader.mfx");
	MShaderAsset* pVShader = m_pAssetRegistryRef->GetShaderAsset(nVShaderId);
	MShaderAsset* pPShader = m_pAssetRegistryRef->GetShaderAsset(nPShaderId);

	MGenericVertex vData[3] = {
		{ 0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
		{ 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
		{ -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}
	};

	uint32_t iData[3] = {
		0, 1, 2
	};

	if (m_pRenderInterface->InitRenderer(m_pWindowRef))
	{
		m_Viewport.TopLeftX = 0.0f;
		m_Viewport.TopLeftY = 0.0f;
		m_Viewport.MaxDepth = 1.0f;
		m_Viewport.MinDepth = 0.0f;
		m_Viewport.Width = (float)m_pWindowRef->GetWidth();
		m_Viewport.Height = (float)m_pWindowRef->GetHeight();

		m_ScissorRect.back = 0.0f;
		m_ScissorRect.front = 0.0f;
		m_ScissorRect.left = 0;
		m_ScissorRect.top = 0;
		m_ScissorRect.right = (float)m_pWindowRef->GetWidth();
		m_ScissorRect.bottom = (float)m_pWindowRef->GetHeight();

		m_pVertexBuffer = m_pRenderInterface->CreateVertexBuffer(vData, 3 * sizeof(MGenericVertex), EBufferUsage::Static);
		m_pIndexBuffer = m_pRenderInterface->CreateIndexBuffer(iData, 3 * sizeof(uint32_t), EBufferUsage::Static);
		m_pPipelineState = m_pRenderInterface->CreatePipelineStateObject(
			pVShader->m_pShaderBytecode, 
			pVShader->m_nShaderBytecodeSize, 
			pPShader->m_pShaderBytecode, 
			pPShader->m_nShaderBytecodeSize);

		return true;
	}
	else
	{
		return false;
	}
}

void C3DRenderer::Destroy()
{
	m_pRenderInterface->WaitForPreviousFrame();

	m_pVertexBuffer->ReleaseBuffer();
	m_pIndexBuffer->ReleaseBuffer();
	m_pPipelineState->ReleaseStateObject();
    m_pRenderInterface->ShutdownRenderer();
}

void C3DRenderer::RenderFrame()
{
	m_pRenderInterface->WaitForPreviousFrame();
	m_pRenderInterface->BindPipelineStateObject(m_pPipelineState);
	m_pRenderInterface->RestartCommandQueue();
	m_pRenderInterface->BindCurrentBackBufferAsRenderTarget();

	m_Viewport.Width = m_pWindowRef->GetWidth();
	m_Viewport.Height = m_pWindowRef->GetHeight();

	m_ScissorRect.right = m_pWindowRef->GetWidth();
	m_ScissorRect.bottom = m_pWindowRef->GetHeight();

	float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f, };
	m_pRenderInterface->ClearRenderTarget(color);

	m_pRenderInterface->SetViewport(m_Viewport, m_ScissorRect);
	m_pRenderInterface->BindVertexBuffer(m_pVertexBuffer);
	m_pRenderInterface->DrawVertices(3);

	m_pRenderInterface->SwapBackBuffers();

	m_pRenderInterface->FinishCommandQueue();
	m_pRenderInterface->ExecuteCommandQueue();
	m_pRenderInterface->PresentBackBuffer();
}
