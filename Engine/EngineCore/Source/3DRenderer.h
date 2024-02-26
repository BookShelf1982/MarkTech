#pragma once
#include "Core.h"
#include "AssetRegistry.h"
#include "VulkanRenderer.h"

struct MPerObjectBuffer
{
	glm::mat4 world;
	glm::vec3 color;
};

struct MPerFrameBuffer
{
	glm::mat4 view;
	glm::mat4 proj;
	glm::vec3 viewPos;
};

class C3DRenderer
{
public:
	C3DRenderer();
	~C3DRenderer();

	void SetRenderAPI(ERendererAPI api);
	bool Init(IWindow* pWindow, CAssetRegistry* pAssetRegistry);
	void Destroy();
	void RenderFrame();
private:
	CAssetRegistry* m_pAssetRegistryRef;
	IWindow* m_pWindowRef;

	MViewport m_Viewport;
	MRect m_ScissorRect;
	IRenderer* m_pRenderInterface;
	IBuffer* m_pVertexBuffer;

	IImage* m_pImage;
	IImageView* m_pImageView;
	IImageSampler* m_pSampler;

	IConstantBuffer* m_pConstantBuffer;
	IConstantBuffer* m_pConstantBuffer2;
	IDescriptorSet* m_pDescriptorSet;
	IDescriptorSet* m_pMaterialDescriptorSet;
	IShader* m_pVertexShader;
	IShader* m_pFragmentShader;
	IPipelineObject* m_pPipeline;
	size_t numInds;
	float upRot;
	glm::mat4 view;
	glm::mat4 proj;
};

