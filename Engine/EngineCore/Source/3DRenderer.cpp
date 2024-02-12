#include "3DRenderer.h"

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
		break;
	case ERendererAPI::Vulkan:
		CreateVulkanRenderer(&m_pRenderInterface);
		break;
	default:
		break;
	}
}

bool C3DRenderer::Init(IWindow* pWindow, CAssetRegistry* pAssetRegistry)
{
	m_pWindowRef = pWindow;
	m_pAssetRegistryRef = pAssetRegistry;
	uint64_t nVShaderId = m_pAssetRegistryRef->LoadShaderAsset("Bin/Shaders/Vert.mfx");
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
		m_pVertexShader = m_pRenderInterface->CreateShader(pVShader->m_pShaderBytecode, pVShader->m_nShaderBytecodeSize);
		m_pFragmentShader = m_pRenderInterface->CreateShader(pPShader->m_pShaderBytecode, pPShader->m_nShaderBytecodeSize);
		m_pPipeline = m_pRenderInterface->CreatePipeline(m_pVertexShader, m_pFragmentShader);

		m_Viewport.TopLeftX = 0.0f;
		m_Viewport.TopLeftY = 0.0f;
		m_Viewport.MaxDepth = 1.0f;
		m_Viewport.MinDepth = 0.0f;
		m_Viewport.Width = (float)m_pWindowRef->GetWidth();
		m_Viewport.Height = (float)m_pWindowRef->GetHeight();

		m_ScissorRect.back = 0;
		m_ScissorRect.front = 0;
		m_ScissorRect.left = 0;
		m_ScissorRect.top = 0;
		m_ScissorRect.right = m_pWindowRef->GetWidth();
		m_ScissorRect.bottom = m_pWindowRef->GetHeight();
		return true;
	}
	else
	{
		return false;
	}
}

void C3DRenderer::Destroy()
{
	m_pVertexShader->ReleaseShader();
	m_pFragmentShader->ReleaseShader();
	m_pPipeline->Release();
	m_pRenderInterface->ShutdownRenderer();
}

void C3DRenderer::RenderFrame()
{
	m_pRenderInterface->WaitForPreviousFrame();

	m_pRenderInterface->AquireNextSwapChainImage();

	m_pRenderInterface->BeginCommandRecording();
	m_pRenderInterface->BindPipelineObject(m_pPipeline);
	m_pRenderInterface->SetViewportRect(m_Viewport);
	m_pRenderInterface->SetScissorRect(m_ScissorRect);
	m_pRenderInterface->DrawVertices(3);
	m_pRenderInterface->EndCommandRecording();
	m_pRenderInterface->SubmitCommandRecording();

	m_pRenderInterface->Present();
}
