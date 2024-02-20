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
	uint64_t nModelId = m_pAssetRegistryRef->LoadModelAsset("bunny.mmdl");
	MShaderAsset* pVShader = m_pAssetRegistryRef->GetShaderAsset(nVShaderId);
	MShaderAsset* pPShader = m_pAssetRegistryRef->GetShaderAsset(nPShaderId);
	MModelAsset* pModelAsset = m_pAssetRegistryRef->GetModelAsset(nModelId);

	if (m_pRenderInterface->InitRenderer(m_pWindowRef))
	{
		m_pVertexShader = m_pRenderInterface->CreateShader(pVShader->m_pShaderBytecode, pVShader->m_nShaderBytecodeSize);
		m_pFragmentShader = m_pRenderInterface->CreateShader(pPShader->m_pShaderBytecode, pPShader->m_nShaderBytecodeSize);
		m_pVertexBuffer = m_pRenderInterface->CreateBuffer((char*)pModelAsset->m_pGeoData, 
			pModelAsset->m_nNumVerts * sizeof(MGenericVertex) + pModelAsset->m_nNumInds * sizeof(uint32_t));
		numInds = pModelAsset->m_nNumInds;

		m_pConstantBuffer = m_pRenderInterface->CreateConstantBuffer(sizeof(MTransformUBuffer), EUsageType::VertexShader);
		m_pPipeline = m_pRenderInterface->CreatePipeline(m_pVertexShader, m_pFragmentShader, &m_pConstantBuffer, 1);

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
		upRot = 0.0f;
		return true;
	}
	else
	{
		return false;
	}
}

void C3DRenderer::Destroy()
{
	m_pRenderInterface->WaitForDeviceToIdle();
	m_pVertexBuffer->ReleaseBuffer();
	m_pVertexShader->ReleaseShader();
	m_pFragmentShader->ReleaseShader();
	m_pConstantBuffer->ReleaseBuffer();
	m_pPipeline->Release();
	m_pRenderInterface->ShutdownRenderer();
}

void C3DRenderer::RenderFrame()
{
	m_pRenderInterface->WaitForPreviousFrame();

	m_pRenderInterface->AquireNextSwapChainImage();

	upRot += 0.001f;

	MTransformUBuffer ubo{};
	ubo.World = glm::rotate(glm::mat4(1.0f), upRot * glm::radians(15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 view = glm::lookAt(glm::vec3(10.0f, 0.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), m_pWindowRef->GetWidth() / (float)m_pWindowRef->GetHeight(), 0.1f, 1000.0f);
	glm::mat4 finalMat = proj * view * ubo.World;
	finalMat[1][1] *= -1;
	ubo.WVP = finalMat;

	m_pRenderInterface->UpdateConstantBuffer(m_pConstantBuffer, &ubo, sizeof(ubo));

	m_pRenderInterface->BeginCommandRecording();
	m_pRenderInterface->BindPipelineObject(m_pPipeline);
	m_pRenderInterface->SetViewportRect(m_Viewport);
	m_pRenderInterface->SetScissorRect(m_ScissorRect);
	m_pRenderInterface->BindVertexBuffer(m_pVertexBuffer, numInds * sizeof(uint32_t));
	m_pRenderInterface->BindIndexBuffer(m_pVertexBuffer, 0);
	m_pRenderInterface->BindConstantBuffer(m_pConstantBuffer);
	m_pRenderInterface->DrawIndices((uint32_t)numInds);
	m_pRenderInterface->EndCommandRecording();
	m_pRenderInterface->SubmitCommandRecording();

	m_pRenderInterface->Present();
}
