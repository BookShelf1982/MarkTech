#pragma once
#ifdef MT_PLATFORM_WINDOWS
#ifndef VK_USE_PLATFORM_WIN32_KHR
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#endif
#include <volk.h>
#include "Math3D\Math3D.h"
#include "Window\Window.h"
#include "DataStructures\Array.h"

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
		VkDescriptorSetLayout perSpriteSetLayout;
		VkDescriptorSetLayout perSpriteImageSetLayout;
		VkDescriptorSetLayout perFrameSetLayout;
	};

	struct ImageImageViewPair
	{
		VkImage image;
		VkImageView imageView;
		VkDescriptorSet imageSet;
	};

	struct DescriptorUniformPair
	{
		VkBuffer uniformBuffer;
		VkDeviceSize memoryOffset;
		VkDescriptorSet descriptorSet;
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
		VkFence finishedUploading;
		MemoryTypeIndicies memoryTypes;
		SpritePipeline spritePipeline;
		VkShaderModule vertexShader;
		VkShaderModule fragmentShader;
		VkDescriptorPool descriptorPool;
		VkSampler imageSampler;
		DeviceStackAllocator textureMemory;
		DeviceStackAllocator hostMemory;
		DescriptorUniformPair perFrameDescriptor;
		DynamicArray<ImageImageViewPair> images;
		DynamicArray<DescriptorUniformPair> spriteDescriptors;
		DynamicArray<VkDescriptorSet> spriteImageSets;
	};

	struct UploadTextureInfo
	{
		void* pData;
		size_t dataSize;
		U32 width, height, channels;
	};

	struct SceneElement
	{
		U32 sprite;
		U32 image;
		Matrix3x3 world;
	};

	struct SceneSnapshot
	{
		DynamicArray<SceneElement> elements;
	};

	bool InitRenderer(const RendererConfig& config, Renderer& renderer);
	U32 UploadTextureToRenderer(Renderer& renderer, const UploadTextureInfo& info);
	U32 CreateSpriteDescriptor(Renderer& renderer, U32 imageIndex);
	void UpdateFrameUniform(Renderer& renderer, const Matrix3x3* pData);
	void UpdateSpriteUniform(Renderer& renderer, U32 spriteDescriptorIndex, const Matrix3x3* pData);
	void RenderFrame(Renderer& renderer, const SceneSnapshot& snapshot);
	void ShutdownRenderer(Renderer& renderer);

}