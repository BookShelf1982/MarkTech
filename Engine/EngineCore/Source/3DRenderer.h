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
	bool Init(GLFWwindow* pWindow);
	void Destroy();
	void RenderFrame();
private:
	IRenderer* m_pRenderInterface;
};

