#include "VulkanImpl.h"
#include "VulkanHelpers.h"
#include <PoolAllocator.h>
#include <Array.h>
#include <Version.h>

#include <stdio.h>

namespace MarkTech
{
	PoolAllocator gCtxAlloc;
	PoolAllocator gSwpAlloc;
	PoolAllocator gObjAlloc;
	PoolAllocator gAdvObjAlloc;
	Array<VkSemaphore> gBlockPresent;
	Array<VkSemaphore> gBlockRender;
	Array<VkSemaphore> gOnFinishedRender;

	static void AbortVulkanContextCreation(VulkanContext* pContext)
	{
		if (pContext->commandPool)
			vkDestroyCommandPool(pContext->device, pContext->commandPool, nullptr);
		if (pContext->device)
			vkDestroyDevice(pContext->device, nullptr);
		if (pContext->messenger)
			vkDestroyDebugUtilsMessengerEXT(pContext->instance, pContext->messenger, nullptr);
		if (pContext->instance)
			vkDestroyInstance(pContext->instance, nullptr);

		volkFinalize();
		FreeToPool(gCtxAlloc, pContext);
		FreePoolAllocator(gCtxAlloc);
	}

	VulkanResultCode CreateVulkanContext(const VulkanContextInfo& info, VulkanContext** ppContext)
	{
		volkInitialize();
		CreatePoolAllocator(gCtxAlloc, sizeof(VulkanContext), 2);

		VulkanContext* pVulkanContext = (VulkanContext*)AllocFromPool(gCtxAlloc);
		*ppContext = pVulkanContext;

		if (CreateInstance(info, &pVulkanContext->instance) != VK_SUCCESS)
		{
			AbortVulkanContextCreation(pVulkanContext);
			return VRC_FAILED;
		}

		if (info.flags & VULKAN_CONTEXT_FLAGS_DEBUG_MESSAGES)
		{
			if (CreateDebugMessenger(pVulkanContext->instance, &pVulkanContext->messenger) != VK_SUCCESS)
			{
				AbortVulkanContextCreation(pVulkanContext);
				return VRC_FAILED;
			}
		}

		VkPhysicalDevice physicalDevice = ChooseBestDevice(pVulkanContext->instance);
		pVulkanContext->physicalDevice = physicalDevice;
		
		if (CreateLogicalDevice(physicalDevice, &pVulkanContext->device, pVulkanContext->queueIndices) != VK_SUCCESS)
		{
			AbortVulkanContextCreation(pVulkanContext);
			return VRC_FAILED;
		}

		vkGetDeviceQueue(pVulkanContext->device, pVulkanContext->queueIndices.graphicsQueue, 0, &pVulkanContext->graphicsQueue);

		VkCommandPoolCreateInfo commandPoolInfo;
		commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolInfo.pNext = nullptr;
		commandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		commandPoolInfo.queueFamilyIndex = pVulkanContext->queueIndices.graphicsQueue;
		
		if (vkCreateCommandPool(pVulkanContext->device, &commandPoolInfo, nullptr, &pVulkanContext->commandPool) != VK_SUCCESS)
		{
			AbortVulkanContextCreation(pVulkanContext);
			return VRC_FAILED;
		}

		VkFenceCreateInfo fenceInfo;
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.pNext = nullptr;
		fenceInfo.flags = 0;

		vkCreateFence(pVulkanContext->device, &fenceInfo, nullptr, &pVulkanContext->finishedPreviousFrame);

		CreatePoolAllocator(gObjAlloc, 8, 1024);
		CreatePoolAllocator(gAdvObjAlloc, 32, 128);
		CreatePoolAllocator(gSwpAlloc, sizeof(VulkanSwapchain), 4);
		ReserveArray(gBlockPresent, 32, nullptr);
		ReserveArray(gOnFinishedRender, 32, nullptr);
		ReserveArray(gBlockRender, 32, nullptr);

		return VRC_SUCCESS;
	}

	void DestroyVulkanContext(VulkanContext* pContext)
	{
		vkDestroyFence(pContext->device, pContext->finishedPreviousFrame, nullptr);
		vkDestroyCommandPool(pContext->device, pContext->commandPool, nullptr);
		vkDestroyDevice(pContext->device, nullptr);

		if (pContext->messenger)
			vkDestroyDebugUtilsMessengerEXT(pContext->instance, pContext->messenger, nullptr);

		vkDestroyInstance(pContext->instance, nullptr);
		volkFinalize();

		DestroyArray(gBlockPresent);
		DestroyArray(gBlockRender);
		DestroyArray(gOnFinishedRender);
		FreeToPool(gCtxAlloc, pContext);
		FreePoolAllocator(gObjAlloc);
		FreePoolAllocator(gAdvObjAlloc);
		FreePoolAllocator(gCtxAlloc);
		FreePoolAllocator(gSwpAlloc);
	}

	void WaitVulkanDeviceIdle(VulkanContext* pContext)
	{
		vkDeviceWaitIdle(pContext->device);
	}

	VulkanResultCode CreateVulkanSwapchain(VulkanContext* pContext, const VulkanSwapchainInfo& info, VulkanSwapchain** ppSwapchain)
	{
		VulkanSwapchain* pSwapchain = (VulkanSwapchain*)AllocFromPool(gSwpAlloc);
		*ppSwapchain = pSwapchain;
		if (CreateSurface(pContext->instance, *info.pWindow, &pSwapchain->surface) != VK_SUCCESS)
		{
			FreeToPool(gObjAlloc, pSwapchain);
			return VRC_FAILED;
		}

		VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
		switch (info.presentationMode)
		{
		case VULKAN_PRESENTATION_MODE_FIFO:
			presentMode = VK_PRESENT_MODE_FIFO_KHR;
			break;
		case VULKAN_PRESENTATION_MODE_IMMEDIATE:
			presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
			break;
		}

		 VkSwapchainKHR oldSwapchain = info.pOldSwapchain ? info.pOldSwapchain->swapchain : nullptr;

		if (CreateVkSwapchain( 
			pContext->device, pContext->physicalDevice, pSwapchain->surface, 
			presentMode, oldSwapchain, pSwapchain
		) != VK_SUCCESS)
		{
			vkDestroySurfaceKHR(pContext->instance, pSwapchain->surface, nullptr);
			FreeToPool(gObjAlloc, pSwapchain);
			return VRC_FAILED;
		}
		
		if (info.pOldSwapchain)
			info.pOldSwapchain->surface = VK_NULL_HANDLE;

		CreateSwapchainRenderPass(pContext->device, pSwapchain);

		vkGetSwapchainImagesKHR(pContext->device, pSwapchain->swapchain, &pSwapchain->swapchainImageCount, pSwapchain->swapchainImages);

		for (U32 i = 0; i < pSwapchain->swapchainImageCount; i++)
		{
			VkImageViewCreateInfo imgViewInfo;
			imgViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			imgViewInfo.pNext = nullptr;
			imgViewInfo.flags = 0;
			imgViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			imgViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			imgViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			imgViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			imgViewInfo.format = pSwapchain->imageFormat;
			imgViewInfo.image = pSwapchain->swapchainImages[i];
			imgViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			imgViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imgViewInfo.subresourceRange.baseArrayLayer = 0;
			imgViewInfo.subresourceRange.baseMipLevel = 0;
			imgViewInfo.subresourceRange.layerCount = 1;
			imgViewInfo.subresourceRange.levelCount = 1;

			vkCreateImageView(pContext->device, &imgViewInfo, nullptr, &pSwapchain->swapchainImageViews[i]);
		}

		for (U32 i = 0; i < pSwapchain->swapchainImageCount; i++)
		{
			VkFramebufferCreateInfo framebufferInfo;
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.pNext = nullptr;
			framebufferInfo.flags = 0;
			framebufferInfo.renderPass = pSwapchain->renderPass;
			framebufferInfo.height = pSwapchain->imageExtent.height;
			framebufferInfo.width = pSwapchain->imageExtent.width;
			framebufferInfo.layers = 1;
			framebufferInfo.pAttachments = &pSwapchain->swapchainImageViews[i];
			framebufferInfo.attachmentCount = 1;

			vkCreateFramebuffer(pContext->device, &framebufferInfo, nullptr, &pSwapchain->swapchainFramebuffers[i].framebuffer);
			pSwapchain->swapchainFramebuffers[i].renderPass = pSwapchain->renderPass;
			pSwapchain->swapchainFramebuffers[i].extent = pSwapchain->imageExtent;
		}

		VkSemaphoreCreateInfo semaphoreInfo;
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		semaphoreInfo.pNext = nullptr;
		semaphoreInfo.flags = 0;

		vkCreateSemaphore(pContext->device, &semaphoreInfo, nullptr, &pSwapchain->acquiredImage);

		return VRC_SUCCESS;
	}

	void DestroyVulkanSwapchain(VulkanContext* pContext, VulkanSwapchain* pSwapchain)
	{
		vkDestroySemaphore(pContext->device, pSwapchain->acquiredImage, nullptr);

		for (U32 i = 0; i < pSwapchain->swapchainImageCount; i++)
			vkDestroyFramebuffer(pContext->device, pSwapchain->swapchainFramebuffers[i].framebuffer, nullptr);

		for (U32 i = 0; i < pSwapchain->swapchainImageCount; i++)
			vkDestroyImageView(pContext->device, pSwapchain->swapchainImageViews[i], nullptr);

		vkDestroyRenderPass(pContext->device, pSwapchain->renderPass, nullptr);
		vkDestroySwapchainKHR(pContext->device, pSwapchain->swapchain, nullptr);
		vkDestroySurfaceKHR(pContext->instance, pSwapchain->surface, nullptr);
		FreeToPool(gObjAlloc, pSwapchain);
	}

	void VulkanSwapchainPresent(VulkanContext* pContext, VulkanSwapchain* pSwapchain)
	{
		VkPresentInfoKHR info;
		info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		info.pNext = nullptr;
		info.swapchainCount = 1;
		info.pSwapchains = &pSwapchain->swapchain;
		info.waitSemaphoreCount = gBlockPresent.size;
		info.pWaitSemaphores = gBlockPresent.size > 0 ? gBlockPresent.pArray : nullptr;
		info.pResults = nullptr;
		U32 indices = pSwapchain->currentImageIndex;
		info.pImageIndices = &indices;

		vkQueuePresentKHR(pContext->graphicsQueue, &info);
		
		EmptyArray(gBlockPresent);

		vkWaitForFences(pContext->device, 1, &pContext->finishedPreviousFrame, VK_TRUE, U64_MAX);
		vkResetFences(pContext->device, 1, &pContext->finishedPreviousFrame);
	}

	VulkanResultCode AcquireNextVulkanFramebuffer(VulkanContext* pContext, VulkanSwapchain* pSwapchain, VulkanFramebuffer** ppFramebuffer)
	{
		vkAcquireNextImageKHR(pContext->device, pSwapchain->swapchain, U64_MAX, pSwapchain->acquiredImage, nullptr, &pSwapchain->currentImageIndex);
		InsertArrayItem(gBlockRender, pSwapchain->acquiredImage);

		*ppFramebuffer = &pSwapchain->swapchainFramebuffers[pSwapchain->currentImageIndex];
		return VRC_SUCCESS;
	}

	VulkanResultCode CreateVulkanCommandBuffer(VulkanContext* pContext, VulkanCommandBuffer** ppCmdBuffer)
	{
		VulkanCommandBuffer* pCmdBuffer = (VulkanCommandBuffer*)AllocFromPool(gAdvObjAlloc);
		*ppCmdBuffer = pCmdBuffer;
		VkCommandBufferAllocateInfo allocateInfo;
		allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocateInfo.pNext = nullptr;
		allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocateInfo.commandBufferCount = 1;
		allocateInfo.commandPool = pContext->commandPool;

		if (vkAllocateCommandBuffers(pContext->device, &allocateInfo, &pCmdBuffer->commandBuffer) != VK_SUCCESS)
			return VRC_FAILED;

		VkSemaphoreCreateInfo semaphoreInfo;
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		semaphoreInfo.pNext = nullptr;
		semaphoreInfo.flags = 0;

		if (vkCreateSemaphore(pContext->device, &semaphoreInfo, nullptr, &pCmdBuffer->finished) != VK_SUCCESS)
			return VRC_FAILED;

		return VRC_SUCCESS;
	}

	void DestroyVulkanCommandBuffer(VulkanContext* pContext, VulkanCommandBuffer* pCmdBuffer)
	{
		vkFreeCommandBuffers(pContext->device, pContext->commandPool, 1, &pCmdBuffer->commandBuffer);
		vkDestroySemaphore(pContext->device, pCmdBuffer->finished, nullptr);
		FreeToPool(gAdvObjAlloc, pCmdBuffer);
	}

	VulkanResultCode StartVulkanCommandRecording(VulkanCommandBuffer* pCmdBuffer)
	{
		VkCommandBufferBeginInfo info;
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		info.flags = 0;
		info.pNext = nullptr;
		info.pInheritanceInfo = nullptr;
		if (vkBeginCommandBuffer(pCmdBuffer->commandBuffer, &info) != VK_SUCCESS)
			return  VRC_FAILED;
		
		return VRC_SUCCESS;
	}

	VulkanResultCode ResetVulkanCommandBuffer(VulkanCommandBuffer* pCmdBuffer)
	{
		if (vkResetCommandBuffer(pCmdBuffer->commandBuffer, 0) != VK_SUCCESS)
			return VRC_FAILED;

		return VRC_SUCCESS;
	}

	VulkanResultCode FinishVulkanCommandBuffer(VulkanCommandBuffer* pCmdBuffer)
	{
		if (vkEndCommandBuffer(pCmdBuffer->commandBuffer) != VK_SUCCESS)
			return VRC_FAILED;

		return VRC_SUCCESS;
	}

	VulkanResultCode SubmitVulkanCommandBuffer(VulkanContext* pContext, VulkanCommandBuffer* pCmdBuffer)
	{
		VkSubmitInfo info;
		info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		info.pNext = nullptr;
		info.commandBufferCount = 1;
		info.pCommandBuffers = &pCmdBuffer->commandBuffer;
		info.signalSemaphoreCount = 1;
		info.pSignalSemaphores = &pCmdBuffer->finished;
		info.waitSemaphoreCount = gBlockRender.size;
		info.pWaitSemaphores = gBlockRender.size ? gBlockRender.pArray : nullptr;
		VkPipelineStageFlags stageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		info.pWaitDstStageMask = &stageMask;

		if (vkQueueSubmit(pContext->graphicsQueue, 1, &info, pContext->finishedPreviousFrame) != VK_SUCCESS)
			return VRC_FAILED;
		
		InsertArrayItem(gBlockPresent, pCmdBuffer->finished);
		EmptyArray(gBlockRender);

		return VRC_SUCCESS;
	}

	void CmdBeginVulkanRenderTarget(VulkanCommandBuffer* pCmdBuffer, VulkanFramebuffer* pFramebuffer)
	{
		VkRenderPassBeginInfo info;
		info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		info.pNext = nullptr;
		info.framebuffer = pFramebuffer->framebuffer;
		info.renderArea.extent = pFramebuffer->extent;
		info.renderArea.offset = { 0,0 };
		info.renderPass = pFramebuffer->renderPass;
		VkClearValue clearColor = { 0.0f, 0.0f, 1.0f, 1.0f };
		info.clearValueCount = 1;
		info.pClearValues = &clearColor;

		vkCmdBeginRenderPass(pCmdBuffer->commandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
	}

	void CmdEndVulkanRenderTarget(VulkanCommandBuffer* pCmdBuffer)
	{
		vkCmdEndRenderPass(pCmdBuffer->commandBuffer);
	}
}