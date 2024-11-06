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

	void WaitDeviceIdle(GraphicsContext context)
	{
		switch (gAPI)
		{
		case GRAPHICS_API_VULKAN:
		{
			WaitVulkanDeviceIdle((VulkanContext*)context);
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

	void SwapchainPresent(GraphicsContext context, Swapchain swapchain)
	{
		switch (gAPI)
		{
		case GRAPHICS_API_VULKAN:
			VulkanSwapchainPresent((VulkanContext*)context, (VulkanSwapchain*)swapchain);
			break;
		}
	}

	ResultCode AcquireNextSwapchainImage(GraphicsContext context, Swapchain swapchain, Framebuffer* pFramebuffer)
	{
		switch (gAPI)
		{
		case GRAPHICS_API_VULKAN:
			return (ResultCode)AcquireNextVulkanFramebuffer((VulkanContext*)context, (VulkanSwapchain*)swapchain, (VulkanFramebuffer**)pFramebuffer);
			break;
		}

		return RC_FAILED;
	}

	ResultCode CreateCommandBuffer(GraphicsContext context, CommandBuffer* pCmdBuffer)
	{
		switch (gAPI)
		{
		case GRAPHICS_API_VULKAN:
			return (ResultCode)CreateVulkanCommandBuffer((VulkanContext*)context, (VulkanCommandBuffer**)pCmdBuffer);
		}

		return RC_FAILED;
	}

	void DestroyCommandBuffer(GraphicsContext context, CommandBuffer cmdBuffer)
	{
		switch (gAPI)
		{
		case GRAPHICS_API_VULKAN:
			DestroyVulkanCommandBuffer((VulkanContext*)context, (VulkanCommandBuffer*)cmdBuffer);
			break;
		}
	}

	ResultCode StartCommandRecording(CommandBuffer cmdBuffer)
	{
		switch (gAPI)
		{
		case GRAPHICS_API_VULKAN:
			return (ResultCode)StartVulkanCommandRecording((VulkanCommandBuffer*)cmdBuffer);
			break;
		}

		return RC_FAILED;
	}

	ResultCode ResetCommandBuffer(CommandBuffer cmdBuffer)
	{
		switch (gAPI)
		{
		case GRAPHICS_API_VULKAN:
			return (ResultCode)ResetVulkanCommandBuffer((VulkanCommandBuffer*)cmdBuffer);
			break;
		}
		
		return RC_FAILED;
	}

	ResultCode FinishCommandBuffer(CommandBuffer cmdBuffer)
	{
		switch (gAPI)
		{
		case GRAPHICS_API_VULKAN:
			return (ResultCode)FinishVulkanCommandBuffer((VulkanCommandBuffer*)cmdBuffer);
			break;
		}

		return RC_FAILED;
	}

	ResultCode SubmitCommandBuffer(GraphicsContext context, CommandBuffer cmdBuffer)
	{
		switch (gAPI)
		{
		case GRAPHICS_API_VULKAN:
			return (ResultCode)SubmitVulkanCommandBuffer((VulkanContext*)context, (VulkanCommandBuffer*)cmdBuffer);
			break;
		}

		return RC_FAILED;
	}

	void CmdBeginRenderTarget(CommandBuffer cmdBuffer, Framebuffer framebuffer)
	{
		switch (gAPI)
		{
		case GRAPHICS_API_VULKAN:
			CmdBeginVulkanRenderTarget((VulkanCommandBuffer*)cmdBuffer, (VulkanFramebuffer*)framebuffer);
			break;
		}
	}

	void CmdEndRenderTarget(CommandBuffer cmdBuffer)
	{
		switch (gAPI)
		{
		case GRAPHICS_API_VULKAN:
			CmdEndVulkanRenderTarget((VulkanCommandBuffer*)cmdBuffer);
			break;
		}
	}
}