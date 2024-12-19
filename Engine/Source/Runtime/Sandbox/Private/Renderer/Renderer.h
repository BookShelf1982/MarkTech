#pragma once
#ifdef MT_PLATFORM_WINDOWS
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#include <volk.h>

#include <Window\Window.h>

namespace MarkTech
{
	enum TextureFiltering
	{
		TEXTURE_FILTERING_NONE,
		TEXTURE_FILTERING_BILINEAR,
		TEXTURE_FILTERING_TRILINEAR,
		TEXTURE_FILTERING_ANISOTRPIC_2X,
		TEXTURE_FILTERING_ANISOTRPIC_4X,
		TEXTURE_FILTERING_ANISOTRPIC_8X,
		TEXTURE_FILTERING_ANISOTRPIC_16X
	};

	enum AntiAliasing
	{
		ANTI_ALIASING_MSAA_2X,
		ANTI_ALIASING_MSAA_4X,
		ANTI_ALIASING_MSAA_8X
	};

	struct RendererConfig
	{
		bool enableFullscreen;
		const Window* pWindow;
		bool enableVSync;
		TextureFiltering textureFiltering;
		AntiAliasing antiAliasing;
	};

	struct Swapchain
	{
		VkSwapchainKHR swapchain;
		VkRenderPass renderPass;
		VkImageView* pImageViews;
		VkFramebuffer* pFramebuffers;
		U32 framebufferCount;
		VkExtent2D extent;
		VkBool32 isFullscreen;
	};

	struct Renderer
	{
#ifdef DEBUG
		VkDebugUtilsMessengerEXT debugMessenger;
#endif
		VkInstance instance;
		VkDevice device;
		VkQueue graphicsQueue;
		U32 graphicsQueueIndex;
		VkSurfaceKHR windowSurface;
		Swapchain swapchain;
		VkCommandPool commandPool;
		VkSemaphore acquiredNextImage;
		VkFence finishedRendering;
	};

	bool InitRenderer(const RendererConfig& config, Renderer& renderer);
	void RenderFrame(Renderer& renderer);
	void ShutdownRenderer(Renderer& renderer);
}