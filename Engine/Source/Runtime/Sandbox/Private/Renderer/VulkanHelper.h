#pragma once
#ifdef MT_PLATFORM_WINDOWS
#ifndef VK_USE_PLATFORM_WIN32_KHR
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#include <Windows.h>
#endif
#include <volk.h>

#include "PrimitiveTypes\intalias.h"

namespace MarkTech
{
	VkResult CreateDebugMessenger(VkInstance instance, VkDebugUtilsMessengerEXT* pMessenger);
	VkResult CreateVulkanInstance(const char* pAppName, U32 appVersion, VkInstance* pInstance);
	VkPhysicalDevice GetSuitableDevice(VkInstance instance, VkSurfaceKHR surface, U32* pGraphicsQueueIndex);
	VkResult CreateVulkanLogicalDevice(VkPhysicalDevice physicalDevice, U32 graphicsQueueIndex, VkDevice* pDevice);
	bool GetGraphicsQueueIndex(VkPhysicalDevice physicalDevice, U32* pQueueIndex);
#ifdef MT_PLATFORM_WINDOWS
	VkResult CreateWin32Surface(VkInstance instance, HWND hwnd, VkSurfaceKHR* pSurface);
#endif
	VkResult CreateVulkanSwapchain(
		VkDevice device,
		VkPhysicalDevice physicalDevice,
		VkSurfaceKHR surface,
		VkPresentModeKHR presentMode,
		VkExtent2D* pExtent,
		VkFormat* pFormat,
		VkSwapchainKHR* pSwapchain
	);

	VkResult CreateVulkanSwapchainFramebuffers(
		VkDevice device,
		VkSwapchainKHR swapchain,
		VkRenderPass swapchainRenderPass,
		VkFormat swapchainFormat,
		VkExtent2D swapchainExtent,
		U32* pImageCount,
		VkFramebuffer** ppFramebuffers,
		VkImageView** ppImageViews
	);

	VkResult CreateVulkanSwapchainRenderPass(VkDevice device, VkFormat swapchainFormat, VkRenderPass* pRenderPass);

	VkResult AllocateVulkanCommandBuffer(VkDevice device, VkCommandPool commandPool, VkCommandBuffer* pBuffer);
	VkResult BeginVulkanCommandBuffer(VkCommandBuffer cmdBuffer);
	void BeginVulkanRenderPass(VkCommandBuffer cmdBuffer, VkRenderPass renderPass, VkFramebuffer framebuffer, VkExtent2D areaExtent);
	void VulkanImageMamoryBarrier(
		VkCommandBuffer cmdBuffer,
		VkPipelineStageFlags srcStageMask,
		VkPipelineStageFlags dstStageMask,
		const VkImageMemoryBarrier* pImageBarrier
	);

	struct VulkanQueueSubmitSyncObjects
	{
		VkPipelineStageFlags* pWaitDstStageFlags;
		U32 waitSemaphoreCount;
		VkSemaphore* pWaitSemaphores;
		U32 signalSemaphoreCount;
		VkSemaphore* pSignalSemaphores;
	};

	VkResult SumbitToVulkanQueue(
		VkQueue queue,
		VkCommandBuffer cmdBuffer,
		const VulkanQueueSubmitSyncObjects* pSyncObj,
		VkFence fence
	);

	VkResult PresentVulkanSwapchain(
		VkQueue queue,
		VkSwapchainKHR swapchain,
		U32 imageIndex,
		VkSemaphore* pWaits,
		U32 waitCount
	);

	VkResult CreateVulkanFence(VkDevice device, VkFenceCreateFlags flags, VkFence* pFence);
	VkResult CreateVulkanSemaphore(VkDevice device, VkSemaphore* pSemaphore);

	VkResult CreateVulkanBuffer(VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage, VkBuffer* pBuffer);
	VkResult CreateVulkanImageView(VkDevice device, VkImage image, VkFormat format, VkImageSubresourceRange subresourceRange, VkImageView* pView);

	VkResult CreateVulkanShader(VkDevice device, U32* pData, U64 size, VkShaderModule* pShader);
	VkResult CreateVulkanDescriptorPool(VkDevice device, U32 maxSets, VkDescriptorPoolSize* pSizes, U32 sizeCount, VkDescriptorPool* pPool);
	VkResult AllocateVulkanDescriptorSet(VkDevice device, VkDescriptorPool pool, VkDescriptorSetLayout layout, VkDescriptorSet* pSet);
}