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
	bool Init(IWindow* pWindow);
	void Destroy();
	void RenderFrame();
private:
	IWindow* m_pWindowRef;
	IRenderer* m_pRenderInterface;
};

