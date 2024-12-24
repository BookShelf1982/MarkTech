#pragma once
#ifdef MT_PLATFORM_WINDOWS
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#include <volk.h>
#include "Math3D\Math3D.h"
#include "Window\Window.h"

namespace MarkTech
{
	struct DeviceStackAllocator
	{
		VkDeviceMemory memory;
		U64 size;
		U64 currentOffset;
	};

	DeviceStackAllocator CreateDeviceStackAllocator(VkDevice device, U32 size, U32 heapIndex);
	VkResult BindBufferToDeviceStack(VkDevice device, VkBuffer buffer, DeviceStackAllocator& allocator);
	VkResult BindImageToDeviceStack(VkDevice device, VkImage image, DeviceStackAllocator& allocator);
	void DestroyDeviceStackAllocator(VkDevice device, DeviceStackAllocator& allocator);

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
	};

	struct MemoryTypeIndicies
	{
		U32 deviceLocalMemory;
		U32 hostVisibleMemory;
	};

	struct SpritePipeline
	{
		VkPipeline pipeline;
		VkPipelineLayout pipelineLayout;
		VkDescriptorSetLayout setLayout;
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
		MemoryTypeIndicies memoryTypes;
		SpritePipeline spritePipeline;
		VkShaderModule vertexShader;
		VkShaderModule fragmentShader;
		VkDescriptorPool descriptorPool;
		VkSampler imageSampler;
		DeviceStackAllocator textureMemory;
		DeviceStackAllocator hostMemory;
		VkDescriptorSet descriptorSet;
		VkImage spriteImage;
		VkImageView spriteImageView;
		VkBuffer uniformBuffer;
	};

	bool InitRenderer(const RendererConfig& config, Renderer& renderer);
	void RenderFrame(Renderer& renderer);
	void ShutdownRenderer(Renderer& renderer);

}