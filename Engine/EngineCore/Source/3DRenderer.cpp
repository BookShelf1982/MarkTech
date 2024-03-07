#include "3DRenderer.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <array>

C3DRenderer::C3DRenderer()
	:m_pRenderInterface(nullptr)
{
}

C3DRenderer::~C3DRenderer()
{
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

bool C3DRenderer::Init(GLFWwindow* pWindow)
{
	if (!m_pRenderInterface->InitRenderer(pWindow))
		return false;

	return true;
}

void C3DRenderer::Destroy()
{
	m_pRenderInterface->ShutdownRenderer();
	delete m_pRenderInterface;
}

void C3DRenderer::RenderFrame()
{
}
