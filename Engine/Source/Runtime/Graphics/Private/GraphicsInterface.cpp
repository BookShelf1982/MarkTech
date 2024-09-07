#include "GraphicsInterface.h"
#include <Version.h>
#include <Log.h>
#include <stdlib.h>
#include <stdio.h>

namespace MarkTech
{
#define MAX_GRAPHICS_DEVICES 4

	VkBool32 DebugMessagengerCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
		VkDebugUtilsMessageTypeFlagsEXT messageTypes, 
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
		void* pUserData)
	{
#ifdef MT_PLATFORM_WINDOWS
		char buffer[1024] = "";
		sprintf_s(buffer, "%s\n", pCallbackData->pMessage);
		OutputDebugStringA(buffer); 
#endif
		return VK_FALSE;
	}

	SwapchainDetails GetSwapchainDetails(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
	{
		SwapchainDetails details = {};
		U32 formatCount = 32;
		U32 modeCount = 6;
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, details.formats);
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &details.caps);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &modeCount, details.modes);
		return details;
	}

	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(VkSurfaceFormatKHR* pFormats, U32 formatCount) 
	{
		for (U32 i = 0; i < formatCount; i++) 
		{
			if (pFormats[i].format == VK_FORMAT_B8G8R8A8_SRGB && pFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
				return pFormats[i];
		}
		return pFormats[0];
	}

	VkPhysicalDevice ChooseBestDevice(VkInstance instance)
	{
		U32 physicalDevicesCount = MAX_GRAPHICS_DEVICES;
		VkPhysicalDevice physicalDevices[MAX_GRAPHICS_DEVICES] = {};
		vkEnumeratePhysicalDevices(instance, &physicalDevicesCount, physicalDevices);

		U32 bestDeviceIndex = 0;
		U32 prevScore = 0;
		for (U32 i = 0; i < physicalDevicesCount; i++)
		{
			U32 score = 0;
			VkPhysicalDeviceProperties deviceProps = {};
			vkGetPhysicalDeviceProperties(physicalDevices[i], &deviceProps);
			if (deviceProps.deviceType = VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
				score += 1024;

			score += deviceProps.limits.maxBoundDescriptorSets;
			score += deviceProps.limits.maxDescriptorSetInputAttachments;
			score += deviceProps.limits.maxDescriptorSetSamplers;
			score += deviceProps.limits.maxDescriptorSetStorageBuffers;
			score += deviceProps.limits.maxDescriptorSetUniformBuffers;

			if (prevScore < score)
			{
				prevScore = score;
				bestDeviceIndex = i;
			}
		}

		return physicalDevices[bestDeviceIndex];
	}

	QueueFamilyIndices GetQueueFamilyIndex(VkPhysicalDevice physicalDevice)
	{
		QueueFamilyIndices indices = {};

		U32 maxProps = 8;
		VkQueueFamilyProperties queueProps[8] = {};
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &maxProps, queueProps);

		U32 queuesLeft = 0;
		for (U32 i = 0; i < maxProps; i++)
		{
			if (queueProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				indices.graphicsQueue = i;
				break;
			}
		}

		return indices;
	}

	GraphicsContext CreateGraphicsContext(const GraphicsContextCreateInfo* pInfo)
	{
		VkResult result = volkInitialize();

		GraphicsContext context = {};

		// Instance Creation
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = pInfo->appInfo.pAppName;
		appInfo.applicationVersion = VK_MAKE_VERSION(pInfo->appInfo.majorVersion, pInfo->appInfo.minorVersion, pInfo->appInfo.patchVersion);
		appInfo.pEngineName = "MarkTech";
		appInfo.engineVersion = VK_MAKE_VERSION(MT_MAJOR_VERSION, MT_MINOR_VERSION, MT_PATCH_VERSION);
		appInfo.apiVersion = VK_VERSION_1_3;

		VkInstanceCreateInfo instanceInfo = {};
		instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceInfo.pApplicationInfo = &appInfo;

		U32 extensionCount = 0;
		char* extensions[8] = {};

		U32 layerCount = 0;
		char* layers[8] = {};

		extensions[extensionCount] = VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME;
		extensionCount++;

		if (pInfo->flags & GRAPHICS_CONTEXT_FLAGS_USE_WINDOW)
		{
			extensions[extensionCount] = VK_KHR_SURFACE_EXTENSION_NAME;
			extensionCount++;
#ifdef MT_PLATFORM_WINDOWS
			extensions[extensionCount] = VK_KHR_WIN32_SURFACE_EXTENSION_NAME;
			extensionCount++;
#endif
		}

		if (pInfo->flags & GRAPHICS_CONTEXT_FLAGS_DEBUG_MESSAGES)
		{
			extensions[extensionCount] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
			extensionCount++;

			layers[layerCount] = "VK_LAYER_KHRONOS_validation";
			layerCount++;
		}

		instanceInfo.ppEnabledExtensionNames = extensions;
		instanceInfo.enabledExtensionCount = extensionCount;
		instanceInfo.ppEnabledLayerNames = layers;
		instanceInfo.enabledLayerCount = layerCount;

		result = vkCreateInstance(&instanceInfo, nullptr, &context.instance);
		if (result != VK_SUCCESS)
		{
			return context;
		}

		volkLoadInstance(context.instance);

		if (pInfo->flags & GRAPHICS_CONTEXT_FLAGS_DEBUG_MESSAGES)
		{
			VkDebugUtilsMessengerCreateInfoEXT debugInfo = {};
			debugInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			debugInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;;
			debugInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;;
			debugInfo.pfnUserCallback = DebugMessagengerCallback;
			if (vkCreateDebugUtilsMessengerEXT(context.instance, &debugInfo, nullptr, &context.messenger) != VK_SUCCESS)
			{
				vkDestroyInstance(context.instance, nullptr);
				volkFinalize();
				context.instance = VK_NULL_HANDLE;
				return context;
			}
		}

		// Device Creation
		context.physicalDevice = ChooseBestDevice(context.instance); // Get best device
		QueueFamilyIndices familyIndices = GetQueueFamilyIndex(context.physicalDevice);

		float defaultPrioity = 1.0f;
		VkDeviceQueueCreateInfo graphicsInfo = {};
		graphicsInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		graphicsInfo.pQueuePriorities = &defaultPrioity;
		graphicsInfo.queueCount = 1;
		graphicsInfo.queueFamilyIndex = familyIndices.graphicsQueue;

		VkDeviceQueueCreateInfo queueInfos[1] = { graphicsInfo };

		// We are reusing the extensions and extensionCount variables from earlier
		extensionCount = 0;
		if (pInfo->flags & GRAPHICS_CONTEXT_FLAGS_USE_WINDOW)
		{
			extensions[extensionCount] = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
			extensionCount++;
		}

		VkDeviceCreateInfo deviceInfo = {};
		deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceInfo.enabledExtensionCount = extensionCount;
		deviceInfo.ppEnabledExtensionNames = extensions;
		deviceInfo.pQueueCreateInfos = queueInfos;
		deviceInfo.queueCreateInfoCount = sizeof(queueInfos) / sizeof(VkDeviceQueueCreateInfo);

		if (vkCreateDevice(context.physicalDevice, &deviceInfo, nullptr, &context.device) != VK_SUCCESS)
		{
			vkDestroyInstance(context.instance, nullptr);
			volkFinalize();
			context.instance = VK_NULL_HANDLE;
			return context;
		}

		vkGetDeviceQueue(context.device, familyIndices.graphicsQueue, 0, &context.graphicsQueue);
		return context;
	}

	void DestroyGraphicsContext(GraphicsContext* pContext)
	{
		vkDestroyDevice(pContext->device, nullptr);
#ifdef DEBUG
		vkDestroyDebugUtilsMessengerEXT(pContext->instance, pContext->messenger, nullptr);
#endif
		vkDestroyInstance(pContext->instance, nullptr);
		volkFinalize();
	}

	GraphicsSemaphore CreateGraphicsSemaphore(const GraphicsContext* pContext)
	{
		GraphicsSemaphore semaphore = {};
		VkSemaphoreCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		vkCreateSemaphore(pContext->device, &info, nullptr, &semaphore.semaphore);
		return semaphore;
	}

	void DestroyGraphicsSemaphore(const GraphicsContext* pContext, GraphicsSemaphore* pSemaphore)
	{
		vkDestroySemaphore(pContext->device, pSemaphore->semaphore, nullptr);
	}

	GraphicsFence CreateGraphicsFence(const GraphicsContext* pContext, bool signaled)
	{
		GraphicsFence fence = {};
		VkFenceCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		if (signaled)
			info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		vkCreateFence(pContext->device, &info, nullptr, &fence.fence);
		return fence;
	}

	void DestroyGraphicsFence(const GraphicsContext* pContext, GraphicsFence* pFence)
	{
		vkDestroyFence(pContext->device, pFence->fence, nullptr);
	}

	void WaitForFences(const GraphicsContext* pContext, GraphicsFence* pFences, U32 fenceCount)
	{
		VkFence fences[MT_MAX_FENCES] = {};
		for (U32 i = 0; i < fenceCount; i++)
			fences[i] = pFences[i].fence;

		vkWaitForFences(pContext->device, fenceCount, fences, VK_TRUE, UINT64_MAX);
	}

	void ResetFences(const GraphicsContext* pContext, GraphicsFence* pFences, U32 fenceCount)
	{
		VkFence fences[MT_MAX_FENCES] = {};
		for (U32 i = 0; i < fenceCount; i++)
			fences[i] = pFences[i].fence;

		vkResetFences(pContext->device, fenceCount, fences);
	}

	Swapchain CreateSwapchain(const GraphicsContext* pContext, const Window* pWindow)
	{
		Swapchain swapchain = {};
#ifdef MT_PLATFORM_WINDOWS
		VkWin32SurfaceCreateInfoKHR surfaceInfo = {};
		surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		surfaceInfo.hinstance = GetModuleHandleA(NULL);
		surfaceInfo.hwnd = pWindow->hWnd;
		if (vkCreateWin32SurfaceKHR(pContext->instance, &surfaceInfo, nullptr, &swapchain.windowSurface) != VK_SUCCESS)
		{
			return swapchain;
		}
#endif
		VkBool32 hasSupport = VK_FALSE;
		vkGetPhysicalDeviceSurfaceSupportKHR(pContext->physicalDevice, pContext->indices.graphicsQueue, swapchain.windowSurface, &hasSupport);
		if (!hasSupport)
		{
			vkDestroySurfaceKHR(pContext->instance, swapchain.windowSurface, nullptr);
			swapchain.swapchain = VK_NULL_HANDLE;
			return swapchain;
		}

		SwapchainDetails details = GetSwapchainDetails(pContext->physicalDevice, swapchain.windowSurface);
		VkSurfaceFormatKHR format = ChooseSwapSurfaceFormat(details.formats, sizeof(details.formats));

		swapchain.framebufferExtent = details.caps.currentExtent;

		VkSwapchainCreateInfoKHR swapchainInfo = {};
		swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainInfo.surface = swapchain.windowSurface;
		swapchainInfo.minImageCount = MT_MAX_SWAPCHAIN_IMAGES;
		swapchainInfo.imageFormat = format.format;
		swapchainInfo.imageColorSpace = format.colorSpace;
		swapchainInfo.imageArrayLayers = 1;
		swapchainInfo.imageExtent = details.caps.currentExtent;
		swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchainInfo.pQueueFamilyIndices = &pContext->indices.graphicsQueue;
		swapchainInfo.queueFamilyIndexCount = 1;
		swapchainInfo.preTransform = details.caps.currentTransform;
		swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainInfo.presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
		swapchainInfo.clipped = VK_TRUE;

		if (vkCreateSwapchainKHR(pContext->device, &swapchainInfo, nullptr, &swapchain.swapchain) != VK_SUCCESS)
		{
			return swapchain;
		}

		vkGetSwapchainImagesKHR(pContext->device, swapchain.swapchain, &swapchain.swapchainImageCount, nullptr);
		vkGetSwapchainImagesKHR(pContext->device, swapchain.swapchain, &swapchain.swapchainImageCount, swapchain.swapchainImages);

		for (U32 i = 0; i < swapchain.swapchainImageCount; i++)
		{
			VkImageViewCreateInfo imageViewInfo = {};
			imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			imageViewInfo.format = format.format;
			imageViewInfo.image = swapchain.swapchainImages[i];
			imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			imageViewInfo.subresourceRange.baseMipLevel = 0;
			imageViewInfo.subresourceRange.baseArrayLayer = 0;
			imageViewInfo.subresourceRange.levelCount = 1;
			imageViewInfo.subresourceRange.layerCount = 1;
			imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;

			vkCreateImageView(pContext->device, &imageViewInfo, nullptr, &swapchain.swapchainImageViews[i]);
		}

		VkAttachmentDescription colorAttatch = {};
		colorAttatch.format = format.format;
		colorAttatch.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttatch.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttatch.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttatch.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttatch.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttatch.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttatch.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorRef = {};
		colorRef.attachment = 0;
		colorRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription colorSubpassDesc = {};
		colorSubpassDesc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		colorSubpassDesc.colorAttachmentCount = 1;
		colorSubpassDesc.pColorAttachments = &colorRef;

		VkSubpassDependency dependSubpass = {};
		dependSubpass.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependSubpass.dstSubpass = 0;
		dependSubpass.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependSubpass.srcAccessMask = 0;
		dependSubpass.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependSubpass.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo renderpassInfo = {};
		renderpassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderpassInfo.pAttachments = &colorAttatch;
		renderpassInfo.attachmentCount = 1;
		renderpassInfo.pSubpasses = &colorSubpassDesc;
		renderpassInfo.subpassCount = 1;
		renderpassInfo.pDependencies = &dependSubpass;
		renderpassInfo.dependencyCount = 1;

		vkCreateRenderPass(pContext->device, &renderpassInfo, nullptr, &swapchain.renderPass);
		
		for (U32 i = 0; i < swapchain.swapchainImageCount; i++)
		{
			VkFramebufferCreateInfo framebufferInfo = {};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = swapchain.renderPass;
			framebufferInfo.pAttachments = &swapchain.swapchainImageViews[i];
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.width = details.caps.currentExtent.width;
			framebufferInfo.height = details.caps.currentExtent.height;
			framebufferInfo.layers = 1;

			vkCreateFramebuffer(pContext->device, &framebufferInfo, nullptr, &swapchain.framebuffers[i]);
		}

		return swapchain;
	}

	void DestroySwapchain(const GraphicsContext* pContext, Swapchain* pSwapchain)
	{
		for (U32 i = 0; i < pSwapchain->swapchainImageCount; i++)
			vkDestroyFramebuffer(pContext->device, pSwapchain->framebuffers[i], nullptr);

		vkDestroyRenderPass(pContext->device, pSwapchain->renderPass, nullptr);

		for (U32 i = 0; i < pSwapchain->swapchainImageCount; i++)
			vkDestroyImageView(pContext->device, pSwapchain->swapchainImageViews[i], nullptr);
		
		vkDestroySwapchainKHR(pContext->device, pSwapchain->swapchain, nullptr);
		vkDestroySurfaceKHR(pContext->instance, pSwapchain->windowSurface, nullptr);
	}

	void AquireNextSwapchainImage(const GraphicsContext* pContext, Swapchain* pSwapchain, GraphicsSemaphore* pSignalSemaphore, GraphicsFence* pSignalFence)
	{
		VkFence fence = pSignalFence ? pSignalFence->fence : VK_NULL_HANDLE;
		VkSemaphore semaphore = pSignalSemaphore ? pSignalSemaphore->semaphore : VK_NULL_HANDLE;
		vkAcquireNextImageKHR(pContext->device, pSwapchain->swapchain, U64_MAX, semaphore, fence, &pSwapchain->framebufferIndex);
	}

	void PresentSwapchainImage(const GraphicsContext* pContext, Swapchain* pSwapchain, GraphicsSemaphore* pWaitSemaphores, U32 waitSemaphoreCount)
	{
		VkPresentInfoKHR info = {};
		info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		info.pImageIndices = &pSwapchain->framebufferIndex;
		info.pSwapchains = &pSwapchain->swapchain;
		info.swapchainCount = 1;

		if (pWaitSemaphores)
		{
			VkSemaphore semaphores[MT_MAX_SEMAPHORES] = {};
			info.pWaitSemaphores = semaphores;
			info.waitSemaphoreCount = waitSemaphoreCount;

			for (U32 i = 0; i < waitSemaphoreCount; i++)
			{
				semaphores[i] = pWaitSemaphores[i].semaphore;
			}
			vkQueuePresentKHR(pContext->graphicsQueue, &info);
			return;
		}

		vkQueuePresentKHR(pContext->graphicsQueue, &info);
	}

	ShaderModule CreateShaderModule(const GraphicsContext* pContext, const ShaderCreateInfo* pInfo)
	{
		ShaderModule shader = {};
		VkShaderModuleCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		info.codeSize = pInfo->sizeInBytes;
		info.pCode = pInfo->pCode;
		vkCreateShaderModule(pContext->device, &info, nullptr, &shader.shader);
		return shader;
	}

	void DestroyShaderModule(const GraphicsContext* pContext, ShaderModule* pShader)
	{
		vkDestroyShaderModule(pContext->device, pShader->shader, nullptr);
	}

	GraphicsPipeline CreateGraphicsPipeline(const GraphicsContext* pContext, const GraphicsPipelineCreateInfo* pInfo)
	{
		//////////////////////////////////////////////////
		/// TODO: use pInfo to configue pipeline creation
		//////////////////////////////////////////////////

		GraphicsPipeline pipeline = {};

		VkPipelineShaderStageCreateInfo shaders[MT_MAX_PIPELINE_SHADERS] = {};
		for (U32 i = 0; i < pInfo->shaderCount; i++)
		{
			VkShaderStageFlagBits shaderStage = VK_SHADER_STAGE_COMPUTE_BIT;
			switch (pInfo->pShaders[i].stage)
			{
			case SHADER_STAGES_FRAGMENT:
				shaderStage = VK_SHADER_STAGE_FRAGMENT_BIT;
				break;
			case SHADER_STAGES_VERTEX:
				shaderStage = VK_SHADER_STAGE_VERTEX_BIT;
				break;
			}

			shaders[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			shaders[i].module = pInfo->pShaders[i].shader.shader;
			shaders[i].pName = pInfo->pShaders[i].pEntrypoint;
			shaders[i].stage = shaderStage;
		}

		VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		if (vkCreatePipelineLayout(pContext->device, &pipelineLayoutInfo, nullptr, &pipeline.pipelineLayout) != VK_SUCCESS)
			return pipeline;

		VkDynamicState state[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
		VkPipelineDynamicStateCreateInfo dynamicStateInfo = {};
		dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicStateInfo.pDynamicStates = state;
		dynamicStateInfo.dynamicStateCount = 2;

		VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo = {};
		inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

		VkPipelineViewportStateCreateInfo viewportStateInfo = {};
		viewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportStateInfo.viewportCount = 1;
		viewportStateInfo.scissorCount = 1;

		VkPipelineRasterizationStateCreateInfo rasterizerInfo = {};
		rasterizerInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizerInfo.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizerInfo.lineWidth = 1.0f;
		rasterizerInfo.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizerInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

		VkPipelineMultisampleStateCreateInfo multisamplingInfo = {};
		multisamplingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisamplingInfo.sampleShadingEnable = VK_FALSE;
		multisamplingInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisamplingInfo.minSampleShading = 1.0f; // Optional
		multisamplingInfo.pSampleMask = nullptr; // Optional
		multisamplingInfo.alphaToCoverageEnable = VK_FALSE; // Optional
		multisamplingInfo.alphaToOneEnable = VK_FALSE; // Optional

		VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_TRUE;
		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		VkPipelineColorBlendStateCreateInfo colorBlending = {};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;

		VkGraphicsPipelineCreateInfo pipelineInfo = {};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.layout = pipeline.pipelineLayout;
		pipelineInfo.pStages = shaders;
		pipelineInfo.stageCount = pInfo->shaderCount;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = &dynamicStateInfo;
		pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
		pipelineInfo.pMultisampleState = &multisamplingInfo;
		pipelineInfo.pRasterizationState = &rasterizerInfo;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pViewportState = &viewportStateInfo;
		pipelineInfo.renderPass = pInfo->renderPass.renderpass;
		pipelineInfo.subpass = 0;

		if (vkCreateGraphicsPipelines(pContext->device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline.pipeline) == VK_SUCCESS)
			return pipeline;

		pipeline.pipelineLayout = VK_NULL_HANDLE;
		vkDestroyPipelineLayout(pContext->device, pipeline.pipelineLayout, nullptr);

		return pipeline;
	}

	void DestroyGraphicsPipeline(const GraphicsContext* pContext, GraphicsPipeline* pPipeline)
	{
		vkDestroyPipeline(pContext->device, pPipeline->pipeline, nullptr);
		vkDestroyPipelineLayout(pContext->device, pPipeline->pipelineLayout, nullptr);
	}

	CommandBufferPool CreateCommandBufferPool(const GraphicsContext* pContext)
	{
		CommandBufferPool pool = {};
		VkCommandPoolCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		info.queueFamilyIndex = pContext->indices.graphicsQueue;
		info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		vkCreateCommandPool(pContext->device, &info, nullptr, &pool.commandPool);
		return pool;
	}

	void DestroyCommandBufferPool(const GraphicsContext* pContext, CommandBufferPool* pPool)
	{
		vkDestroyCommandPool(pContext->device, pPool->commandPool, nullptr);
	}

	void ResetCommandBufferPool(const GraphicsContext* pContext, CommandBufferPool* pPool)
	{
		vkResetCommandPool(pContext->device, pPool->commandPool, 0);
	}	
	
	CommandBuffer AllocateCommandBuffer(const GraphicsContext* pContext, const CommandBufferPool* pPool)
	{
		CommandBuffer buffer = {};
		VkCommandBufferAllocateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		info.commandBufferCount = 1;
		info.commandPool = pPool->commandPool;
		info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		vkAllocateCommandBuffers(pContext->device, &info, &buffer.commandBuffer);
		return buffer;
	}

	void ResetCommandBuffer(CommandBuffer* pCmdBuffer)
	{
		vkResetCommandBuffer(pCmdBuffer->commandBuffer, 0);
	}

	void FreeCommandBuffer(const GraphicsContext* pContext, CommandBuffer* pCmdBuffer, const CommandBufferPool* pPool)
	{
		vkFreeCommandBuffers(pContext->device, pPool->commandPool, 1, &pCmdBuffer->commandBuffer);
	}

	void BeginCommandBufferRecording(CommandBuffer* pCmdBuffer)
	{
		VkCommandBufferBeginInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		vkBeginCommandBuffer(pCmdBuffer->commandBuffer, &info);
	}

	void EndCommandBufferRecording(CommandBuffer* pCmdBuffer)
	{
		vkEndCommandBuffer(pCmdBuffer->commandBuffer);
	}

	void SubmitCommandBuffer(const GraphicsContext* pContext,
		CommandBuffer* pCmdBuffer,
		GraphicsSemaphore* pSignalSemaphores, U32 signalSemaphoreCount,
		GraphicsSemaphore* pWaitSemaphores, U32 waitSemaphoreCount,
		GraphicsFence* pFence)
	{
		VkSubmitInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		info.pCommandBuffers = &pCmdBuffer->commandBuffer;
		info.commandBufferCount = 1;

		VkFence fence = pFence ? pFence->fence : VK_NULL_HANDLE;
		if (pSignalSemaphores || pWaitSemaphores)
		{
			VkSemaphore signalSemaphores[MT_MAX_SEMAPHORES] = {};
			VkSemaphore waitSemaphores[MT_MAX_SEMAPHORES] = {};
			info.pSignalSemaphores = signalSemaphores;
			info.signalSemaphoreCount = signalSemaphoreCount;
			info.pWaitSemaphores = waitSemaphores;
			info.waitSemaphoreCount = waitSemaphoreCount;
			///////////////////////////////////////////////
			/// TODO: Make pWaitDstStageMask a paramater
			///////////////////////////////////////////////
			VkPipelineStageFlags mask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			info.pWaitDstStageMask = &mask;

			for (U32 i = 0; i < waitSemaphoreCount; i++)
				waitSemaphores[i] = pWaitSemaphores[i].semaphore;

			for (U32 i = 0; i < signalSemaphoreCount; i++)
				signalSemaphores[i] = pSignalSemaphores[i].semaphore;
			
			vkQueueSubmit(pContext->graphicsQueue, 1, &info, fence);
			return;
		}

		vkQueueSubmit(pContext->graphicsQueue, 1, &info, fence);
	}

	void CmdBindSwapchainFramebuffer(CommandBuffer* pCmdBuffer, const Swapchain* pSwapchain)
	{
		VkRenderPassBeginInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		info.clearValueCount = 1;
		VkClearValue value = {};
		info.pClearValues = &value;
		info.framebuffer = pSwapchain->framebuffers[pSwapchain->framebufferIndex];
		info.renderPass = pSwapchain->renderPass;
		info.renderArea.extent = pSwapchain->framebufferExtent;
		info.renderArea.offset = { 0, 0 };

		vkCmdBeginRenderPass(pCmdBuffer->commandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
	}

	void CmdEndFramebuffer(CommandBuffer* pCmdBuffer)
	{
		vkCmdEndRenderPass(pCmdBuffer->commandBuffer);
	}

	void CmdBindPipeline(CommandBuffer* pCmdBuffer, const GraphicsPipeline* pPipeline)
	{
		vkCmdBindPipeline(pCmdBuffer->commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pPipeline->pipeline);
	}

	void CmdDraw(CommandBuffer* pCmdBuffer, U32 offset, U32 length)
	{
		vkCmdDraw(pCmdBuffer->commandBuffer, length, 1, offset, 0);
	}

	void CmdSetViewportScissor(CommandBuffer* pCmdBuffer, const ViewportScissor* pViewportScissor)
	{
		VkViewport viewport = {};
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		viewport.height = pViewportScissor->height;
		viewport.width = pViewportScissor->width;
		viewport.x = pViewportScissor->x;
		viewport.y = pViewportScissor->y;
		
		VkRect2D rect = {};
		rect.offset = { pViewportScissor->offsetX, pViewportScissor->offsetY };
		rect.extent.width = pViewportScissor->extentX;
		rect.extent.height = pViewportScissor->extentY;

		vkCmdSetViewport(pCmdBuffer->commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(pCmdBuffer->commandBuffer, 0, 1, &rect);
	}
}