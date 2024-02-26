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
	uint64_t nModelId = m_pAssetRegistryRef->LoadModelAsset("uv_sphere.mmdl");
	MShaderAsset* pVShader = m_pAssetRegistryRef->GetShaderAsset(nVShaderId);
	MShaderAsset* pPShader = m_pAssetRegistryRef->GetShaderAsset(nPShaderId);
	MModelAsset* pModelAsset = m_pAssetRegistryRef->GetModelAsset(nModelId);

	int texWidth, texHeight, texChannels;
	byte* pTexData = stbi_load("texture.jpg", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
	size_t nTexDataSize = texWidth * texHeight * 4;

	if (m_pRenderInterface->InitRenderer(m_pWindowRef))
	{
		m_pVertexShader = m_pRenderInterface->CreateShader(pVShader->m_pShaderBytecode, pVShader->m_nShaderBytecodeSize);
		m_pFragmentShader = m_pRenderInterface->CreateShader(pPShader->m_pShaderBytecode, pPShader->m_nShaderBytecodeSize);
		m_pVertexBuffer = m_pRenderInterface->CreateBuffer(
			(char*)pModelAsset->m_pGeoData, 
			pModelAsset->m_nNumVerts * sizeof(MGenericVertex) + pModelAsset->m_nNumInds * sizeof(uint32_t));
		numInds = pModelAsset->m_nNumInds;
		m_pConstantBuffer = m_pRenderInterface->CreateConstantBuffer(sizeof(MPerFrameBuffer), EUsageType::VertexShader);
		m_pConstantBuffer2 = m_pRenderInterface->CreateConstantBuffer(sizeof(MPerObjectBuffer), EUsageType::VertexShader);

		m_pImage = m_pRenderInterface->CreateImage((char*)pTexData, nTexDataSize, (size_t)texWidth, (size_t)texHeight);
		m_pImageView = m_pRenderInterface->CreateImageView(m_pImage);
		m_pSampler = m_pRenderInterface->CreateImageSmpler(m_pImageView);

		stbi_image_free(pTexData);

		MDescriptorSetLayoutBindingDesc texBinding{};
		texBinding.binding = 3;
		texBinding.descriptorCount = 1;
		texBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		texBinding.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

		MDescriptorSetLayoutDesc texLayout{};
		texLayout.bindingCount = 1;
		texLayout.pBindings = &texBinding;

		MDesciptorImageDesc texImageDesc{};
		texImageDesc.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		texImageDesc.pImageSampler = m_pSampler;
		texImageDesc.pImageView = m_pImageView;

		MDescriptorDesc texDesc{};
		texDesc.binding = 3;
		texDesc.type = IMAGE_SAMPLER;
		texDesc.pImageInfo = &texImageDesc;

		m_pMaterialDescriptorSet = m_pRenderInterface->CreateDescriptorSet(&texDesc, 1, texLayout);

		MDescriptorSetLayoutBindingDesc bindingDesc{};
		bindingDesc.binding = 0;
		bindingDesc.descriptorCount = 1;
		bindingDesc.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_VERTEX_BIT;
		bindingDesc.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

		MDescriptorSetLayoutBindingDesc bindingDesc2{};
		bindingDesc2.binding = 1;
		bindingDesc2.descriptorCount = 1;
		bindingDesc2.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		bindingDesc2.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

		std::array<MDescriptorSetLayoutBindingDesc, 2> bindings = {
			bindingDesc,
			bindingDesc2
		};

		MDescriptorSetLayoutDesc layoutDesc{};
		layoutDesc.bindingCount = (uint32_t)bindings.size();
		layoutDesc.pBindings = bindings.data();

		MDescriptorBufferDesc bufferInfo{};
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(MPerFrameBuffer);
		bufferInfo.pBuffer = m_pConstantBuffer;

		MDescriptorBufferDesc bufferInfo2{};
		bufferInfo2.offset = 0;
		bufferInfo2.range = sizeof(MPerObjectBuffer);
		bufferInfo2.pBuffer = m_pConstantBuffer2;

		MDescriptorDesc setDesc{};
		setDesc.binding = 0;
		setDesc.type = UNIFORM_BUFFER;
		setDesc.pBufferInfo = &bufferInfo;

		MDescriptorDesc setDesc2{};
		setDesc2.binding = 1;
		setDesc2.type = UNIFORM_BUFFER;
		setDesc2.pBufferInfo = &bufferInfo2;

		std::array<MDescriptorDesc, 2> setsDescs = {
			setDesc,
			setDesc2
		};

		m_pDescriptorSet = m_pRenderInterface->CreateDescriptorSet(setsDescs.data(), setsDescs.size(), layoutDesc);

		std::vector<IDescriptorSet*> descSets = {
			m_pDescriptorSet,
			m_pMaterialDescriptorSet
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
		view[1][1] *= -1;
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
	m_pMaterialDescriptorSet->ReleaseDescriptorSet();
	m_pImage->ReleaseImage();
	m_pImageView->ReleaseImageView();
	m_pSampler->ReleaseSampler();
	m_pVertexBuffer->ReleaseBuffer();
	m_pVertexShader->ReleaseShader();
	m_pFragmentShader->ReleaseShader();
	m_pConstantBuffer2->ReleaseBuffer();
	m_pConstantBuffer->ReleaseBuffer();
	m_pPipeline->Release();
	m_pRenderInterface->ShutdownRenderer();
}

void C3DRenderer::RenderFrame()
{
	m_pRenderInterface->WaitForPreviousFrame();

	m_pRenderInterface->AquireNextSwapChainImage();

	upRot += 0.001f;
	MPerFrameBuffer perFrame{};
	perFrame.proj = proj;
	perFrame.view = view;
	perFrame.viewPos = glm::vec3(10.0f, 0.0f, 2.0f);

	MPerObjectBuffer perObject{};
	perObject.world = glm::rotate(glm::mat4(1.0f), upRot * glm::radians(15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	perObject.color = glm::vec3(0.25f, 0.25f, 0.25f);

	m_pRenderInterface->UpdateConstantBuffer(m_pConstantBuffer, &perFrame, sizeof(perFrame));
	m_pRenderInterface->UpdateConstantBuffer(m_pConstantBuffer2, &perObject, sizeof(perObject));

	m_pRenderInterface->BeginCommandRecording();
	m_pRenderInterface->BindPipelineObject(m_pPipeline);
	m_pRenderInterface->SetViewportRect(m_Viewport);
	m_pRenderInterface->SetScissorRect(m_ScissorRect);
	m_pRenderInterface->BindVertexBuffer(m_pVertexBuffer, numInds * sizeof(uint32_t));
	m_pRenderInterface->BindIndexBuffer(m_pVertexBuffer, 0);
	std::vector<IDescriptorSet*> descSets = {
		m_pDescriptorSet,
		m_pMaterialDescriptorSet
	};
	m_pRenderInterface->BindDescriptorSets(descSets.data(), descSets.size());
	m_pRenderInterface->DrawIndices((uint32_t)numInds);
	m_pRenderInterface->EndCommandRecording();
	m_pRenderInterface->SubmitCommandRecording();

	m_pRenderInterface->Present();
}
