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
	
	int texWidth, texHeight, texChannels;
	byte* pImageData = stbi_load("texture.jpg", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
	size_t imageSize = texWidth * texHeight * 4;

	if (m_pRenderInterface->InitRenderer(m_pWindowRef))
	{
		m_pVertexShader = m_pRenderInterface->CreateShader(pVShader->m_pShaderBytecode, pVShader->m_nShaderBytecodeSize);
		m_pFragmentShader = m_pRenderInterface->CreateShader(pPShader->m_pShaderBytecode, pPShader->m_nShaderBytecodeSize);
		m_pVertexBuffer = m_pRenderInterface->CreateBuffer(
			(char*)pModelAsset->m_pGeoData, 
			pModelAsset->m_nNumVerts * sizeof(MGenericVertex) + pModelAsset->m_nNumInds * sizeof(uint32_t));
		numInds = pModelAsset->m_nNumInds;
		m_pTexture = m_pRenderInterface->CreateImage((char*)pImageData, imageSize, texWidth, texHeight);
		m_pTextureView = m_pRenderInterface->CreateImageView(m_pTexture);
		m_pTextureSampler = m_pRenderInterface->CreateImageSmpler(m_pTextureView);
		stbi_image_free(pImageData);
		m_pConstantBuffer = m_pRenderInterface->CreateConstantBuffer(sizeof(MTransformUBuffer), EUsageType::VertexShader);

		MDescriptorSetLayoutBindingDesc bindingDesc{};
		bindingDesc.binding = 0;
		bindingDesc.descriptorCount = 1;
		bindingDesc.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		bindingDesc.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

		MDescriptorSetLayoutBindingDesc samplerBindingDesc{};
		samplerBindingDesc.binding = 1;
		samplerBindingDesc.descriptorCount = 1;
		samplerBindingDesc.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerBindingDesc.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		std::array<MDescriptorSetLayoutBindingDesc, 2> bindings = {
			bindingDesc,
			samplerBindingDesc
		};

		MDescriptorSetLayoutDesc layoutDesc{};
		layoutDesc.bindingCount = (uint32_t)bindings.size();
		layoutDesc.pBindings = bindings.data();

		MDescriptorBufferDesc bufferInfo{};
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(MTransformUBuffer);
		bufferInfo.pBuffer = m_pConstantBuffer;

		MDesciptorImageDesc imageDesc{};
		imageDesc.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageDesc.pImageView = m_pTextureView;
		imageDesc.pImageSampler = m_pTextureSampler;

		MDescriptorDesc setDesc{};
		setDesc.binding = 0;
		setDesc.type = UNIFORM_BUFFER;
		setDesc.pBufferInfo = &bufferInfo;

		MDescriptorDesc imageSetDesc{};
		imageSetDesc.binding = 1;
		imageSetDesc.type = IMAGE_SAMPLER;
		imageSetDesc.pImageInfo = &imageDesc;

		std::array<MDescriptorDesc, 2> setsDescs = {
			setDesc,
			imageSetDesc
		};

		m_pDescriptorSet = m_pRenderInterface->CreateDescriptorSet(setsDescs.data(), setsDescs.size(), layoutDesc);

		std::vector<IDescriptorSet*> descSets = {
			m_pDescriptorSet
		};

		m_pPipeline = m_pRenderInterface->CreatePipeline(m_pVertexShader, m_pFragmentShader, descSets.data(), descSets.size());

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
		view = glm::lookAt(glm::vec3(10.0f, 0.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		proj = glm::perspective(glm::radians(45.0f), m_pWindowRef->GetWidth() / (float)m_pWindowRef->GetHeight(), 0.1f, 1000.0f);
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
	m_pDescriptorSet->ReleaseDescriptorSet();
	m_pTexture->ReleaseImage();
	m_pTextureView->ReleaseImageView();
	m_pTextureSampler->ReleaseSampler();
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
	std::vector<IDescriptorSet*> descSets = {
		m_pDescriptorSet
	};
	m_pRenderInterface->BindDescriptorSets(descSets.data(), descSets.size());
	m_pRenderInterface->DrawIndices((uint32_t)numInds);
	m_pRenderInterface->EndCommandRecording();
	m_pRenderInterface->SubmitCommandRecording();

	m_pRenderInterface->Present();
}
