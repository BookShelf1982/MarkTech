#include "VulkanImpl.h"
#include "VulkanHelpers.h"
#include <PoolAllocator.h>
#include <Version.h>
#include <Log.h>
#include <HashMap.h>

#include <stdio.h>

namespace MarkTech
{
	PoolAllocator gCtxAlloc;
	PoolAllocator gSwpAlloc;
	PoolAllocator gObjAlloc;
	PoolAllocator gAdvObjAlloc;
	Array<VkSemaphore> gBlockPresent;
	Array<VkSemaphore> gBlockRender;
	PoolAllocator gPipelineLayoutNodeAlloc;
	HashMap<U32, VkPipelineLayout> gPipelineLayouts;

	static void GetPipelineLayout(VkDevice device, const VkPipelineLayoutCreateInfo* pInfo, VkPipelineLayout* pLayout)
	{
		U32 key = 0x1;
		if (GetHashMapItem(gPipelineLayouts, key, pLayout))
			return;

		VkPipelineLayout layout = VK_NULL_HANDLE;
		vkCreatePipelineLayout(device, pInfo, nullptr, &layout);
		InsertHashMapItem(gPipelineLayouts, key, layout);
		*pLayout = layout;
	}

	static void DestroyAllPipelineLayouts(VkDevice device)
	{
		for (U32 i = 0; i < gPipelineLayouts.map.reservedSize; i++)
		{
			LinkedList<HashMap<U32, VkPipelineLayout>::KeyValType>::NodeType* pNode = gPipelineLayouts.map.pArray[i].pStart;
			while (pNode != nullptr)
			{
				vkDestroyPipelineLayout(device, pNode->data.value, nullptr);
				pNode = pNode->pNext;
			}
		}
	}

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
		else
		{
			pVulkanContext->messenger = nullptr;
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
		CreatePoolAllocator(gPipelineLayoutNodeAlloc, 24, 16);
		ReserveArray(gBlockPresent, 32, nullptr);
		ReserveArray(gBlockRender, 32, nullptr);
		AllocatorInfo allocInfo;
		allocInfo.pMemAlloc = nullptr;
		allocInfo.pPoolAlloc = &gPipelineLayoutNodeAlloc;
		CreateHashMap(gPipelineLayouts, 16, &allocInfo);

		return VRC_SUCCESS;
	}

	void DestroyVulkanContext(VulkanContext* pContext)
	{
		DestroyAllPipelineLayouts(pContext->device);
		vkDestroyFence(pContext->device, pContext->finishedPreviousFrame, nullptr);
		vkDestroyCommandPool(pContext->device, pContext->commandPool, nullptr);
		vkDestroyDevice(pContext->device, nullptr);

		if (pContext->messenger)
			vkDestroyDebugUtilsMessengerEXT(pContext->instance, pContext->messenger, nullptr);

		vkDestroyInstance(pContext->instance, nullptr);
		volkFinalize();

		DestroyArray(gBlockPresent);
		DestroyArray(gBlockRender);
		DestroyHashMap(gPipelineLayouts);
		FreeToPool(gCtxAlloc, pContext);
		FreePoolAllocator(gPipelineLayoutNodeAlloc);
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

		vkGetSwapchainImagesKHR(pContext->device, pSwapchain->swapchain, &pSwapchain->swapchainImageCount, nullptr);
		ReserveArray(pSwapchain->swapchainImages, pSwapchain->swapchainImageCount, nullptr);
		ReserveArray(pSwapchain->swapchainImageViews, pSwapchain->swapchainImageCount, nullptr);
		ReserveArray(pSwapchain->swapchainFramebuffers, pSwapchain->swapchainImageCount, nullptr);
		vkGetSwapchainImagesKHR(pContext->device, pSwapchain->swapchain, &pSwapchain->swapchainImageCount, pSwapchain->swapchainImages.pArray);

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
			imgViewInfo.image = pSwapchain->swapchainImages.pArray[i];
			imgViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			imgViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imgViewInfo.subresourceRange.baseArrayLayer = 0;
			imgViewInfo.subresourceRange.baseMipLevel = 0;
			imgViewInfo.subresourceRange.layerCount = 1;
			imgViewInfo.subresourceRange.levelCount = 1;

			vkCreateImageView(pContext->device, &imgViewInfo, nullptr, &pSwapchain->swapchainImageViews.pArray[i]);
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
			framebufferInfo.pAttachments = &pSwapchain->swapchainImageViews.pArray[i];
			framebufferInfo.attachmentCount = 1;

			vkCreateFramebuffer(pContext->device, &framebufferInfo, nullptr, &pSwapchain->swapchainFramebuffers.pArray[i].framebuffer);
			pSwapchain->swapchainFramebuffers.pArray[i].renderPass = pSwapchain->renderPass;
			pSwapchain->swapchainFramebuffers.pArray[i].extent = pSwapchain->imageExtent;
		}

		VkSemaphoreCreateInfo semaphoreInfo;
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		semaphoreInfo.pNext = nullptr;
		semaphoreInfo.flags = 0;

		vkCreateSemaphore(pContext->device, &semaphoreInfo, nullptr, &pSwapchain->acquiredImage);

		VkFenceCreateInfo fenceInfo;
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.pNext = nullptr;
		fenceInfo.flags = 0;

		vkCreateFence(pContext->device, &fenceInfo, nullptr, &pSwapchain->acquiredImageFence);

		return VRC_SUCCESS;
	}

	void DestroyVulkanSwapchain(VulkanContext* pContext, VulkanSwapchain* pSwapchain)
	{
		vkDestroyFence(pContext->device, pSwapchain->acquiredImageFence, nullptr);
		vkDestroySemaphore(pContext->device, pSwapchain->acquiredImage, nullptr);

		for (U32 i = 0; i < pSwapchain->swapchainImageCount; i++)
			vkDestroyFramebuffer(pContext->device, pSwapchain->swapchainFramebuffers.pArray[i].framebuffer, nullptr);

		for (U32 i = 0; i < pSwapchain->swapchainImageCount; i++)
			vkDestroyImageView(pContext->device, pSwapchain->swapchainImageViews.pArray[i], nullptr);

		vkDestroyRenderPass(pContext->device, pSwapchain->renderPass, nullptr);
		vkDestroySwapchainKHR(pContext->device, pSwapchain->swapchain, nullptr);
		vkDestroySurfaceKHR(pContext->instance, pSwapchain->surface, nullptr);
		DestroyArray(pSwapchain->swapchainFramebuffers);
		DestroyArray(pSwapchain->swapchainImages);
		DestroyArray(pSwapchain->swapchainImageViews);
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
		
		//vkWaitForFences(pContext->device, 1, &pSwapchain->acquiredImageFence, VK_TRUE, U64_MAX);
		vkResetFences(pContext->device, 1, &pSwapchain->acquiredImageFence);

		*ppFramebuffer = &pSwapchain->swapchainFramebuffers.pArray[pSwapchain->currentImageIndex];
		return VRC_SUCCESS;
	}

	void GetVulkanSwapchainFramebufferClass(VulkanSwapchain* pSwapchain, U64* fbClass)
	{
		*fbClass = (U64)pSwapchain->renderPass;
	}

	void GetVulkanSwapchainExtent(VulkanSwapchain* pSwapchain, VkExtent2D* pExtent)
	{
		*pExtent = pSwapchain->imageExtent;
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
		if (!pFramebuffer->framebuffer)
			return;

		if (!pFramebuffer->renderPass)
			return;

		VkRenderPassBeginInfo info;
		info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		info.pNext = nullptr;
		info.framebuffer = pFramebuffer->framebuffer;
		info.renderArea.extent = pFramebuffer->extent;
		info.renderArea.offset = { 0,0 };
		info.renderPass = pFramebuffer->renderPass;
		VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
		info.clearValueCount = 1;
		info.pClearValues = &clearColor;

		vkCmdBeginRenderPass(pCmdBuffer->commandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
	}

	void CmdEndVulkanRenderTarget(VulkanCommandBuffer* pCmdBuffer)
	{
		vkCmdEndRenderPass(pCmdBuffer->commandBuffer);
	}

	VulkanResultCode CreateVulkanShader(VulkanContext* pContext, const VulkanShaderCreateInfo& info, VulkanShader** ppShader)
	{
		VulkanShader* pShader = (VulkanShader*)AllocFromPool(gObjAlloc);
		*ppShader = pShader;
		VkShaderModuleCreateInfo shaderInfo;
		shaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		shaderInfo.pNext = nullptr;
		shaderInfo.flags = 0;
		shaderInfo.pCode = info.pSPIR;
		shaderInfo.codeSize = info.sizeInBytes;
		VkResult result = vkCreateShaderModule(pContext->device, &shaderInfo, nullptr, &pShader->shader);
		if (result != VK_SUCCESS)
			return VRC_FAILED;

		return VRC_SUCCESS;
	}

	void DestroyVulkanShader(VulkanContext* pContext, VulkanShader* pShader)
	{
		vkDestroyShaderModule(pContext->device, pShader->shader, nullptr);
	}

	VulkanResultCode CreateVulkanPipeline(VulkanContext* pContext, const VulkanPipelineCreateInfo& info, VulkanPipeline** ppPipeline)
	{
		VulkanPipeline* pPipeline = (VulkanPipeline*)AllocFromPool(gObjAlloc);
		*ppPipeline = pPipeline;

		VkPipelineLayoutCreateInfo layoutInfo = {};
		layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		VkPipelineLayout layout = VK_NULL_HANDLE;
		GetPipelineLayout(pContext->device, &layoutInfo, &layout);

		Array<VkVertexInputBindingDescription> bindingDescs = {};
		Array<VkVertexInputAttributeDescription> attribDescs = {};
		Array<VkPipelineShaderStageCreateInfo> shadersInfos = {};
		
		GetVertexInfoFromInfo(info, attribDescs, bindingDescs);
		GetShaderStageInfoFromInfo(info, shadersInfos);

		VkPipelineVertexInputStateCreateInfo vertexInfo = {};
		vertexInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInfo.vertexAttributeDescriptionCount = attribDescs.size;
		vertexInfo.pVertexAttributeDescriptions = attribDescs.size > 0 ? attribDescs.pArray : nullptr;
		vertexInfo.vertexBindingDescriptionCount = bindingDescs.size;
		vertexInfo.pVertexBindingDescriptions = bindingDescs.size > 0 ? bindingDescs.pArray : nullptr;
		
		VkPipelineInputAssemblyStateCreateInfo inputInfo = {};
		inputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		switch (info.topology)
		{
		case VULKAN_TOPOLOGY_TRIANGLE_LIST:
			inputInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			break;
		}

		VkPipelineViewportStateCreateInfo viewportState = {};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.scissorCount = 1;
		viewportState.viewportCount = 1;

		VkPipelineRasterizationStateCreateInfo rasterizationInfo = {};
		rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizationInfo.cullMode = (U32)info.pRasterizationInfo->cullMode;
		rasterizationInfo.lineWidth = info.pRasterizationInfo->lineWidth;
		rasterizationInfo.frontFace = (VkFrontFace)info.pRasterizationInfo->frontFace;
		rasterizationInfo.polygonMode = (VkPolygonMode)info.pRasterizationInfo->polygonMode;

		switch (info.pRasterizationInfo->polygonMode)
		{
		case VULKAN_POLYGON_MODE_FILL:
			rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
			break;
		case VULKAN_POLYGON_MODE_LINE:
			rasterizationInfo.polygonMode = VK_POLYGON_MODE_LINE;
			break;
		case VULKAN_POLYGON_MODE_POINT:
			rasterizationInfo.polygonMode = VK_POLYGON_MODE_POINT;
			break;
		}

		VkDynamicState states[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
		VkPipelineDynamicStateCreateInfo dynamicState = {};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = sizeof(states) / sizeof(VkDynamicState);
		dynamicState.pDynamicStates = states;

		VkPipelineMultisampleStateCreateInfo multisampleInfo = {};
		multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
		colorBlendAttachment.blendEnable = VK_FALSE;
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

		VkPipelineColorBlendStateCreateInfo colorBlendInfo = {};
		colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlendInfo.attachmentCount = 1;
		colorBlendInfo.pAttachments = &colorBlendAttachment;

		VkGraphicsPipelineCreateInfo pipelineInfo = {};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.pVertexInputState = &vertexInfo;
		pipelineInfo.pInputAssemblyState = &inputInfo;
		pipelineInfo.pRasterizationState = &rasterizationInfo;
		pipelineInfo.pDynamicState = &dynamicState;
		pipelineInfo.pMultisampleState = &multisampleInfo;
		pipelineInfo.pColorBlendState = &colorBlendInfo;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.stageCount = shadersInfos.size;
		pipelineInfo.pStages = shadersInfos.size > 0 ? shadersInfos.pArray : nullptr;
		pipelineInfo.renderPass = info.renderPass;
		pipelineInfo.subpass = 0;
		pipelineInfo.layout = layout;
		
		VulkanResultCode result = VRC_SUCCESS;
		if (vkCreateGraphicsPipelines(pContext->device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pPipeline->pipeline) != VK_SUCCESS)
			result = VRC_FAILED;

		DestroyArray(bindingDescs);
		DestroyArray(attribDescs);
		DestroyArray(shadersInfos);

		return result;
	}

	void DestroyVulkanPipeline(VulkanContext* pContext, VulkanPipeline* pPipeline)
	{
		vkDestroyPipeline(pContext->device, pPipeline->pipeline, nullptr);
	}

	void CmdBindVulkanPipeline(VulkanCommandBuffer* pCmdBuffer, VulkanPipeline* pPipeline)
	{
		vkCmdBindPipeline(pCmdBuffer->commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pPipeline->pipeline);
	}

	void CmdVulkanDrawVertices(VulkanCommandBuffer* pCmdBuffer, U32 offset, U32 size)
	{
		vkCmdDraw(pCmdBuffer->commandBuffer, size, 1, offset, 0);
	}

	void CmdSetVulkanViewport(VulkanCommandBuffer* pCmdBuffer, VkViewport viewport)
	{
		vkCmdSetViewport(pCmdBuffer->commandBuffer, 0, 1, &viewport);
	}

	void CmdSetVulkanScissor(VulkanCommandBuffer* pCmdBuffer, VkRect2D rect)
	{
		vkCmdSetScissor(pCmdBuffer->commandBuffer, 0, 1, &rect);
	}
}
