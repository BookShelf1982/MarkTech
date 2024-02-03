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
	m_pRenderInterface->ShutdownRenderer();
}

void C3DRenderer::RenderFrame()
{
	
}
