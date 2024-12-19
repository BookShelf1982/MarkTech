#include "Renderer.h"
#include "VulkanHelper.h"

namespace MarkTech
{
	bool CreateRendererSwapchain(
		VkDevice device,
		VkPhysicalDevice physicalDevice,
		VkSurfaceKHR surface,
		const RendererConfig& config,
		Swapchain& swapchain
	)
	{
		VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
		if (!config.enableVSync)
			presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;

		VkFormat swapchainFormat;

		if (CreateVulkanSwapchain(
			device,
			physicalDevice,
			surface,
			presentMode,
			&swapchain.extent,
			&swapchainFormat,
			&swapchain.swapchain
		) != VK_SUCCESS)
		{
			return false;
		}

		if (CreateVulkanSwapchainRenderPass(device, swapchainFormat, &swapchain.renderPass) != VK_SUCCESS)
			return false;

		if (CreateVulkanSwapchainFramebuffers(
			device,
			swapchain.swapchain,
			swapchain.renderPass,
			swapchainFormat,
			swapchain.extent,
			&swapchain.framebufferCount,
			&swapchain.pFramebuffers,
			&swapchain.pImageViews
		) != VK_SUCCESS)
		{
			return false;
		}
		
		if (config.enableFullscreen)
		{
			VkResult result = vkAcquireFullScreenExclusiveModeEXT(device, swapchain.swapchain);
			if (result != VK_SUCCESS)
				swapchain.isFullscreen = VK_TRUE;
		}

		return true;
	}

	bool InitRenderer(const RendererConfig& config, Renderer& renderer)
	{
		if (CreateVulkanInstance("Pac-Man", VK_MAKE_VERSION(0, 0, 1), &renderer.instance) != VK_SUCCESS)
			return false;

#ifdef DEBUG
		CreateDebugMessenger(renderer.instance, &renderer.debugMessenger);
#endif

#ifdef MT_PLATFORM_WINDOWS
		if (CreateWin32Surface(renderer.instance, config.pWindow->hWnd, &renderer.windowSurface) != VK_SUCCESS)
			return false;
#endif
		VkPhysicalDevice physicalDevice = GetSuitableDevice(renderer.instance, renderer.windowSurface, &renderer.graphicsQueueIndex);

		if (CreateVulkanLogicalDevice(physicalDevice, renderer.graphicsQueueIndex, &renderer.device) != VK_SUCCESS)
			return false;

		vkGetDeviceQueue(renderer.device, renderer.graphicsQueueIndex, 0, &renderer.graphicsQueue);

		if (!CreateRendererSwapchain(
			renderer.device,
			physicalDevice,
			renderer.windowSurface,
			config,
			renderer.swapchain)
			)
		{
			return false;
		}

		VkCommandPoolCreateInfo commandPoolInfo;
		commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolInfo.pNext = nullptr;
		commandPoolInfo.flags = 0;
		commandPoolInfo.queueFamilyIndex = renderer.graphicsQueueIndex;
		
		if (vkCreateCommandPool(renderer.device, &commandPoolInfo, nullptr, &renderer.commandPool) != VK_SUCCESS)
			return false;

		CreateVulkanFence(renderer.device, 0, &renderer.finishedRendering);
		CreateVulkanSemaphore(renderer.device, &renderer.acquiredNextImage);

		return true;
	}

	void RenderFrame(Renderer& renderer)
	{
		VkCommandBuffer cmdBuffer;
		AllocateVulkanCommandBuffer(renderer.device, renderer.commandPool, &cmdBuffer);
		BeginVulkanCommandBuffer(cmdBuffer);

		U32 imageIndex = 0;
		vkAcquireNextImageKHR(renderer.device, renderer.swapchain.swapchain, UINT64_MAX, renderer.acquiredNextImage, VK_NULL_HANDLE, &imageIndex);

		BeginVulkanRenderPass(cmdBuffer, renderer.swapchain.renderPass, renderer.swapchain.pFramebuffers[imageIndex], renderer.swapchain.extent);

		vkCmdEndRenderPass(cmdBuffer);
		vkEndCommandBuffer(cmdBuffer);

		VulkanQueueSubmitSyncObjects syncObjects = {};
		VkPipelineStageFlags waitStageDst = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		syncObjects.pWaitDstStageFlags = &waitStageDst;
		syncObjects.pWaitSemaphores = &renderer.acquiredNextImage;
		syncObjects.waitSemaphoreCount = 1;

		SumbitToVulkanQueue(renderer.graphicsQueue, cmdBuffer, &syncObjects, renderer.finishedRendering);
		
		vkWaitForFences(renderer.device, 1, &renderer.finishedRendering, VK_TRUE, UINT64_MAX);
		vkResetFences(renderer.device, 1, &renderer.finishedRendering);
		PresentVulkanSwapchain(renderer.graphicsQueue, renderer.swapchain.swapchain, imageIndex, nullptr, 0);

		vkFreeCommandBuffers(renderer.device, renderer.commandPool, 1, &cmdBuffer);
	}

	void ShutdownRenderer(Renderer& renderer)
	{
		if (renderer.swapchain.isFullscreen)
			vkReleaseFullScreenExclusiveModeEXT(renderer.device, renderer.swapchain.swapchain);

		vkDeviceWaitIdle(renderer.device);

		vkDestroySemaphore(renderer.device, renderer.acquiredNextImage, nullptr);
		vkDestroyFence(renderer.device, renderer.finishedRendering, nullptr);

		vkDestroyCommandPool(renderer.device, renderer.commandPool, nullptr);

		for (U32 i = 0; i < renderer.swapchain.framebufferCount; i++)
		{
			vkDestroyFramebuffer(renderer.device, renderer.swapchain.pFramebuffers[i], nullptr);
			vkDestroyImageView(renderer.device, renderer.swapchain.pImageViews[i], nullptr);
		}
		vkDestroyRenderPass(renderer.device, renderer.swapchain.renderPass, nullptr);
		vkDestroySwapchainKHR(renderer.device, renderer.swapchain.swapchain, nullptr);

		vkDestroyDevice(renderer.device, nullptr);
		vkDestroySurfaceKHR(renderer.instance, renderer.windowSurface, nullptr);
#ifdef DEBUG
		vkDestroyDebugUtilsMessengerEXT(renderer.instance, renderer.debugMessenger, nullptr);
#endif
		vkDestroyInstance(renderer.instance, nullptr);
		volkFinalize();

		free(renderer.swapchain.pFramebuffers);
		free(renderer.swapchain.pImageViews);
	}
}