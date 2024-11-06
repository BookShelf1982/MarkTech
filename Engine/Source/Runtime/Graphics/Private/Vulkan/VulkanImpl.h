#pragma once
#include <PrimitiveTypes.h>
#ifdef MT_PLATFORM_WINDOWS
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#include "Window.h"
#include <volk.h>

namespace MarkTech
{
	enum VulkanContextCreateFlagBits
	{
		VULKAN_CONTEXT_FLAGS_DEBUG_MESSAGES = 0x1
	};

	enum VulkanResultCode
	{
		VRC_SUCCESS,
		VRC_FAILED
	};

	typedef U32 VulkanContextCreateFlags;

	struct VulkanApplicationInfo
	{
		const char* pName;
		U32 verMajor;
		U32 verMinor;
		U32 verPatch;
	};

	struct QueueFamilyIndices
	{
		U32 graphicsQueue;
	};

	struct VulkanContextInfo
	{
		VulkanApplicationInfo* pAppInfo;
		VulkanContextCreateFlags flags;
	};

	struct VulkanContext
	{
		VkInstance instance;
		VkDevice device;
		VkQueue graphicsQueue;
		VkPhysicalDevice physicalDevice;
		VkDebugUtilsMessengerEXT messenger;
		QueueFamilyIndices queueIndices;
		VkCommandPool commandPool;
		VkFence finishedPreviousFrame;
	};

	VulkanResultCode CreateVulkanContext(const VulkanContextInfo& info, VulkanContext** ppContext);
	void DestroyVulkanContext(VulkanContext* pContext);
	void WaitVulkanDeviceIdle(VulkanContext* pContext);

	struct VulkanFramebuffer
	{
		VkRenderPass renderPass;
		VkFramebuffer framebuffer;
		VkExtent2D extent;
	};

	enum VulkanPresentationMode
	{
		VULKAN_PRESENTATION_MODE_FIFO,
		VULKAN_PRESENTATION_MODE_IMMEDIATE
	};

	struct VulkanSwapchain
	{
		VkSurfaceKHR surface;
		VkSwapchainKHR swapchain;
		VkImage swapchainImages[3];
		VkImageView swapchainImageViews[3];
		VulkanFramebuffer swapchainFramebuffers[3];
		VkRenderPass renderPass;
		VkFormat imageFormat;
		VkExtent2D imageExtent;
		VkSemaphore acquiredImage;
		VkSemaphore finishedRendering;
		U32 swapchainImageCount;
		U32 currentImageIndex;
	};

	struct VulkanSwapchainInfo
	{
		Window* pWindow;
		VulkanPresentationMode presentationMode;
		VulkanSwapchain* pOldSwapchain;
	};

	VulkanResultCode CreateVulkanSwapchain(VulkanContext* pContext, const VulkanSwapchainInfo& info, VulkanSwapchain** ppSwapchain);
	void DestroyVulkanSwapchain(VulkanContext* pContext, VulkanSwapchain* pSwapchain);
	void VulkanSwapchainPresent(VulkanContext* pContext, VulkanSwapchain* pSwapchain);
	VulkanResultCode AcquireNextVulkanFramebuffer(VulkanContext* pContext, VulkanSwapchain* pSwapchain, VulkanFramebuffer** ppFramebuffer);

	struct VulkanCommandBuffer
	{
		VkCommandBuffer commandBuffer;
		VkSemaphore finished;
	};

	VulkanResultCode CreateVulkanCommandBuffer(VulkanContext* pContext, VulkanCommandBuffer** ppCmdBuffer);
	void DestroyVulkanCommandBuffer(VulkanContext* pContext, VulkanCommandBuffer* pCmdBuffer);
	VulkanResultCode StartVulkanCommandRecording(VulkanCommandBuffer* pCmdBuffer);
	VulkanResultCode ResetVulkanCommandBuffer(VulkanCommandBuffer* pCmdBuffer);
	VulkanResultCode FinishVulkanCommandBuffer(VulkanCommandBuffer* pCmdBuffer);
	VulkanResultCode SubmitVulkanCommandBuffer(VulkanContext* pContext, VulkanCommandBuffer* pCmdBuffer);

	void CmdBeginVulkanRenderTarget(VulkanCommandBuffer* pCmdBuffer, VulkanFramebuffer* pFramebuffer);
	void CmdEndVulkanRenderTarget(VulkanCommandBuffer* pCmdBuffer);
}