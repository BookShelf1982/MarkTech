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

	void GetSwapchainFramebufferClass(Swapchain swapchain, FramebufferClass fbClass)
	{
		switch (gAPI)
		{
		case GRAPHICS_API_VULKAN:
			GetVulkanSwapchainFramebufferClass((VulkanSwapchain*)swapchain, (U64*)fbClass);
			break;
		}
	}

	void GetSwapchainExtent(Swapchain swapchain, Extent2D* pExtent)
	{
		switch (gAPI)
		{
		case GRAPHICS_API_VULKAN:
			GetVulkanSwapchainExtent((VulkanSwapchain*)swapchain, (VkExtent2D*)pExtent);
			break;
		}
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

	ResultCode CreateShader(GraphicsContext context, const ShaderCreateInfo& info, Shader* pShader)
	{
		switch (gAPI)
		{
		case GRAPHICS_API_VULKAN:
		{
			VulkanShaderCreateInfo vulkanInfo = {
				info.pSPIR,
				info.sizeInBytes
			};
			return (ResultCode)CreateVulkanShader((VulkanContext*)context, vulkanInfo, (VulkanShader**)pShader);
		} break;
		}

		return RC_FAILED;
	}

	void DestroyShader(GraphicsContext context, Shader shader)
	{
		switch (gAPI)
		{
		case GRAPHICS_API_VULKAN:
			DestroyVulkanShader((VulkanContext*)context, (VulkanShader*)shader);
			break;
		}
	}

	ResultCode CreateGraphicsPipeline(GraphicsContext context, const GraphicsPipelineCreateInfo& info, GraphicsPipeline* pPipeline)
	{
		switch (gAPI)
		{
		case GRAPHICS_API_VULKAN:
			return (ResultCode)CreateVulkanPipeline((VulkanContext*)context, (const VulkanPipelineCreateInfo&)info, (VulkanPipeline**)pPipeline);
			break;
		}

		return RC_FAILED;
	}

	void DestroyGraphicsPipeline(GraphicsContext context, GraphicsPipeline pipeline)
	{
		switch (gAPI)
		{
		case GRAPHICS_API_VULKAN:
			DestroyVulkanPipeline((VulkanContext*)context, (VulkanPipeline*)pipeline);
			break;
		}
	}

	void CmdBindPipeline(CommandBuffer cmdBuffer, GraphicsPipeline pipeline)
	{
		switch (gAPI)
		{
		case GRAPHICS_API_VULKAN:
			CmdBindVulkanPipeline((VulkanCommandBuffer*)cmdBuffer, (VulkanPipeline*)pipeline);
			break;
		}
	}

	void CmdDrawVertices(CommandBuffer cmdBuffer, U32 offset, U32 size)
	{
		switch (gAPI)
		{
		case GRAPHICS_API_VULKAN:
			CmdVulkanDrawVertices((VulkanCommandBuffer*)cmdBuffer, offset, size);
			break;
		}
	}

	void CmdSetViewport(CommandBuffer cmdBuffer, Viewport viewport)
	{
		switch (gAPI)
		{
		case GRAPHICS_API_VULKAN:
			VkViewport vulkanViewport;
			vulkanViewport.height = viewport.height;
			vulkanViewport.width = viewport.width;
			vulkanViewport.maxDepth = viewport.maxDepth;
			vulkanViewport.minDepth = viewport.minDepth;
			vulkanViewport.x = viewport.x;
			vulkanViewport.y = viewport.y;
			CmdSetVulkanViewport((VulkanCommandBuffer*)cmdBuffer, vulkanViewport);
			break;
		}
	}

	void CmdSetScissor(CommandBuffer cmdBuffer, Rect2D rect)
	{
		switch (gAPI)
		{
		case GRAPHICS_API_VULKAN:
			VkRect2D vulkanRect;
			vulkanRect.extent.width = rect.width;
			vulkanRect.extent.height = rect.height;
			vulkanRect.offset.x = rect.x;
			vulkanRect.offset.y = rect.y;
			CmdSetVulkanScissor((VulkanCommandBuffer*) cmdBuffer, vulkanRect);
			break;
		}
	}
}