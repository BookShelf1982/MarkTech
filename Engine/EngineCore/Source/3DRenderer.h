#pragma once
#include "Core.h"
#include "AssetRegistry.h"
#include "Renderer.h"
#include "VulkanRenderer.h"

struct MPerObjectCBuffer
{
	DirectX::XMMATRIX WVP;
	DirectX::XMMATRIX World;
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
	IVertexBuffer* m_pVertexBuffer;
	IIndexBuffer* m_pIndexBuffer;
	IPipelineState* m_pPipelineState;
};

