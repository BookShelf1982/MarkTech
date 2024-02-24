#pragma once
#include "Core.h"
#include "AssetRegistry.h"
#include "VulkanRenderer.h"

struct MTransformUBuffer
{
	glm::mat4 WVP;
	glm::mat4 World;
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
	IImage* m_pTexture;
	IImageView* m_pTextureView;
	IImageSampler* m_pTextureSampler;
	IConstantBuffer* m_pConstantBuffer;
	IDescriptorSet* m_pDescriptorSet;
	IShader* m_pVertexShader;
	IShader* m_pFragmentShader;
	IPipelineObject* m_pPipeline;
	size_t numInds;
	float upRot;
	glm::mat4 view;
	glm::mat4 proj;
};

