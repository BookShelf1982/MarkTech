#include "GraphicsInterface.h"
#include "Vulkan\VulkanImpl.h"

namespace MarkTech
{
	GraphicsAPI gAPI;

	ResultCode CreateGraphicsContext(const GraphicsContextCreateInfo& info, GraphicsContext* pContext)
	{
		switch (info.api)
		{
		case GRAPHICS_API_VULKAN:
		{
			gAPI = GRAPHICS_API_VULKAN;
			VulkanContextInfo vulkanInfo;
			vulkanInfo.flags = info.flags;
			VulkanApplicationInfo appInfo;
			appInfo.pName = info.pAppInfo->pName;
			appInfo.verMajor = info.pAppInfo->verMajor;
			appInfo.verMinor = info.pAppInfo->verMinor;
			appInfo.verPatch = info.pAppInfo->verPatch;
			vulkanInfo.pAppInfo = &appInfo;
			return (ResultCode)CreateVulkanContext(vulkanInfo, (VulkanContext**)pContext);
		} break;
		}

		return RC_FAILED;
	}

	void DestroyGraphicsContext(GraphicsContext context)
	{
		switch (gAPI)
		{
		case GRAPHICS_API_VULKAN:
		{
			DestroyVulkanContext((VulkanContext*)context);
		} break;
		}
	}

	ResultCode CreateSwapchain(GraphicsContext context, const SwapchainCreateInfo& info, Swapchain* pSwapchain)
	{
		switch (gAPI)
		{
		case GRAPHICS_API_VULKAN:
			VulkanSwapchainInfo swapchainInfo = {
				info.pWindow,
				(VulkanPresentationMode)info.presentationMode,
				(VulkanSwapchain*)info.oldSwapchain
			};
			return (ResultCode)CreateVulkanSwapchain((VulkanContext*)context, swapchainInfo, (VulkanSwapchain**)pSwapchain);
			break;
		}

		return RC_FAILED;
	}

	void DestroySwapchain(GraphicsContext context, Swapchain swapchain)
	{
		switch (gAPI)
		{
		case GRAPHICS_API_VULKAN:
			DestroyVulkanSwapchain((VulkanContext*)context, (VulkanSwapchain*)swapchain);
			break;
		}
	}
}