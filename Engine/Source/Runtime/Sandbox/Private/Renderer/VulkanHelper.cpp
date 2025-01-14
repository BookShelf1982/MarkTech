#include "VulkanHelper.h"
#include <PrimitiveTypes\intalias.h>

#include <stdlib.h>
#include <stdio.h>

#define ENGINE_NAME "MarkTech"
#define ENGINE_VERSION VK_MAKE_VERSION(1,0,0)

namespace MarkTech
{
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

	VkResult CreateVulkanInstance(const char* pAppName, U32 appVersion, VkInstance* pInstance)
	{
		VkResult result = volkInitialize();
		if (result != VK_SUCCESS)
			return result;

		U32 version = VK_API_VERSION_1_2;
		vkEnumerateInstanceVersion(&version);

		VkApplicationInfo appInfo;
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pNext = nullptr;
		appInfo.pEngineName = ENGINE_NAME;
		appInfo.pApplicationName = pAppName;
		appInfo.engineVersion = ENGINE_VERSION;
		appInfo.applicationVersion = appVersion;
		appInfo.apiVersion = version;

		const char* extensions[] = {
			VK_KHR_SURFACE_EXTENSION_NAME,
#ifdef MT_PLATFORM_WINDOWS
			VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#endif
#ifdef DEBUG
			VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
#endif
		};

		VkInstanceCreateInfo info;
		info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = 0;
		info.pApplicationInfo = &appInfo;
		info.ppEnabledExtensionNames = extensions;
		info.enabledExtensionCount = sizeof(extensions) / sizeof(const char*);

#ifdef DEBUG
		const char* layers[] = {
			"VK_LAYER_KHRONOS_validation"
		};

		info.ppEnabledLayerNames = layers;
		info.enabledLayerCount = sizeof(layers) / sizeof(const char*);
#else
		info.enabledLayerCount = 0;
		info.ppEnabledLayerNames = nullptr;
#endif

		result = vkCreateInstance(&info, nullptr, pInstance);
		if (result != VK_SUCCESS)
		{
			volkFinalize();
			return result;
		}

		volkLoadInstance(*pInstance);

		return result;
	}

	VkResult CreateDebugMessenger(VkInstance instance, VkDebugUtilsMessengerEXT* pMessenger)
	{
		VkDebugUtilsMessengerCreateInfoEXT debugInfo = {};
		debugInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;
		debugInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		debugInfo.pfnUserCallback = DebugMessagengerCallback;
		return vkCreateDebugUtilsMessengerEXT(instance, &debugInfo, nullptr, pMessenger);
	}

	VkPhysicalDevice GetSuitableDevice(VkInstance instance, VkSurfaceKHR surface, U32* pGraphicsQueueIndex)
	{
		U32 deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
		VkPhysicalDevice* pDevices = (VkPhysicalDevice*)malloc(sizeof(VkPhysicalDevice) * deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, pDevices);

		U32 graphicsQueueIndex = 0;
		U32 highScore = 0;
		VkPhysicalDevice bestDevice = VK_NULL_HANDLE;

		// Check if physical devices are suitable
		for (U32 i = 0; i < deviceCount; i++)
		{
			U32 score = 0;
			GetGraphicsQueueIndex(pDevices[i], &graphicsQueueIndex);

			VkBool32 supportsSurface = VK_FALSE;
			vkGetPhysicalDeviceSurfaceSupportKHR(pDevices[i], graphicsQueueIndex, surface, &supportsSurface);
			if (!supportsSurface)
				continue; // Skip scoring phase if it doesn't meet requirements

			VkPhysicalDeviceProperties deviceProps;
			vkGetPhysicalDeviceProperties(pDevices[i], &deviceProps);
			if (deviceProps.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
				score += 1000;

			if (score > highScore)
			{
				highScore = score;
				bestDevice = pDevices[i];
			}
		}

		*pGraphicsQueueIndex = graphicsQueueIndex;
		free(pDevices);
		
		return bestDevice;
	}

	bool GetGraphicsQueueIndex(VkPhysicalDevice physicalDevice, U32* pQueueIndex)
	{
		U32 propCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &propCount, nullptr);
		VkQueueFamilyProperties* pProps = (VkQueueFamilyProperties*)malloc(sizeof(VkQueueFamilyProperties) * propCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &propCount, pProps);

		for (U32 i = 0; i < propCount; i++)
		{
#ifdef MT_PLATFORM_WINDOWS
			VkBool32 supportsPresentation = vkGetPhysicalDeviceWin32PresentationSupportKHR(physicalDevice, i);
#endif
			if (pProps[i].queueFlags & (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT) && supportsPresentation)
			{
				free(pProps);
				*pQueueIndex = i;
				return true;
			}
		}

		return false;
	}

	VkResult CreateVulkanLogicalDevice(VkPhysicalDevice physicalDevice, U32 graphicsQueueIndex, VkDevice* pDevice)
	{
		float priority = 1.0f;
		VkDeviceQueueCreateInfo graphicsQueue;
		graphicsQueue.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		graphicsQueue.pNext = nullptr;
		graphicsQueue.flags = 0;
		graphicsQueue.queueCount = 1;
		graphicsQueue.pQueuePriorities = &priority;
		graphicsQueue.queueFamilyIndex = graphicsQueueIndex;

		const char* extensions[] = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		VkPhysicalDeviceFeatures deviceFeatures = {};

		VkDeviceCreateInfo info;
		info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = 0;
		info.queueCreateInfoCount = 1;
		info.pQueueCreateInfos = &graphicsQueue;
		info.enabledExtensionCount = sizeof(extensions) / sizeof(const char*);
		info.ppEnabledExtensionNames = extensions;
		info.enabledLayerCount = 0;
		info.ppEnabledLayerNames = nullptr;
		info.pEnabledFeatures = &deviceFeatures;
		
		return vkCreateDevice(physicalDevice, &info, nullptr, pDevice);
	}

#ifdef MT_PLATFORM_WINDOWS
	VkResult CreateWin32Surface(VkInstance instance, HWND hwnd, VkSurfaceKHR* pSurface)
	{
		VkWin32SurfaceCreateInfoKHR info;
		info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		info.pNext = nullptr;
		info.flags = 0;
		info.hinstance = GetModuleHandleA(NULL);
		info.hwnd = hwnd;

		return vkCreateWin32SurfaceKHR(instance, &info, nullptr, pSurface);
	}
#endif

	VkSurfaceFormatKHR FindSurfaceFormat(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
	{
		U32 count = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &count, nullptr);
		VkSurfaceFormatKHR* pSurfaceFormats = (VkSurfaceFormatKHR*)malloc(sizeof(VkSurfaceFormatKHR) * count);
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &count, pSurfaceFormats);

		VkSurfaceFormatKHR surfaceFormat = pSurfaceFormats[0];
		for (U32 i = 0; i < count; i++)
		{
			if (pSurfaceFormats[i].format == VK_FORMAT_B8G8R8A8_UNORM && pSurfaceFormats[i].colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR)
			{
				surfaceFormat = pSurfaceFormats[i];
				break;
			}
		}
		
		free(pSurfaceFormats);
		return surfaceFormat;
	}

	VkPresentModeKHR GetPresentMode(VkPhysicalDevice device, VkSurfaceKHR surface, VkPresentModeKHR desiredPresentMode)
	{
		U32 count = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &count, nullptr);
		VkPresentModeKHR* pPresentModes = (VkPresentModeKHR*)malloc(sizeof(VkPresentModeKHR) * count);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &count, pPresentModes);

		VkPresentModeKHR presentMode = pPresentModes[0];
		for (U32 i = 0; i < count; i++)
		{
			if (pPresentModes[i] == desiredPresentMode)
			{
				presentMode = pPresentModes[i];
				break;
			}
		}

		free(pPresentModes);
		return presentMode;
	}

	VkResult CreateVulkanSwapchain(
		VkDevice device,
		VkPhysicalDevice physicalDevice,
		VkSurfaceKHR surface,
		VkPresentModeKHR presentMode,
		VkExtent2D* pExtent,
		VkFormat* pFormat,
		VkSwapchainKHR* pSwapchain
	)
	{
		VkSurfaceCapabilitiesKHR surfaceCaps;
		VkSurfaceFormatKHR surfaceFormat = FindSurfaceFormat(physicalDevice, surface);
		*pFormat = surfaceFormat.format;

		VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCaps);
		*pExtent = surfaceCaps.currentExtent;

		if (!(surfaceCaps.supportedUsageFlags & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT))
			return VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR;

		VkSwapchainCreateInfoKHR info;
		info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		info.pNext = nullptr;
		info.flags = 0;
		info.surface = surface;
		info.minImageCount = surfaceCaps.minImageCount > 2 ? surfaceCaps.minImageCount : 2;
		info.imageFormat = surfaceFormat.format;
		info.imageColorSpace = surfaceFormat.colorSpace;
		info.imageExtent = surfaceCaps.currentExtent;
		info.imageArrayLayers = 1;
		info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		info.queueFamilyIndexCount = 0;
		info.pQueueFamilyIndices = nullptr;
		info.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		info.presentMode = GetPresentMode(physicalDevice, surface, presentMode);
		info.clipped = VK_TRUE;
		info.oldSwapchain = VK_NULL_HANDLE;

		result = vkCreateSwapchainKHR(device, &info, nullptr, pSwapchain);
		return result;
	}

	VkResult CreateVulkanSwapchainFramebuffers(
		VkDevice device,
		VkSwapchainKHR swapchain,
		VkRenderPass swapchainRenderPass,
		VkFormat swapchainFormat,
		VkExtent2D swapchainExtent,
		U32* pImageCount,
		VkFramebuffer** ppFramebuffers,
		VkImageView** ppImageViews
	)
	{
		U32 imageCount = 0;
		vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr);
		VkImage* pImages = (VkImage*)malloc(sizeof(VkImage) * imageCount);
		VkImageView* pImageViews = (VkImageView*)malloc(sizeof(VkImageView) * imageCount);
		VkFramebuffer* pFramebuffers = (VkFramebuffer*)malloc(sizeof(VkFramebuffer) * imageCount);
		vkGetSwapchainImagesKHR(device, swapchain, &imageCount, pImages);

		for (U32 i = 0; i < imageCount; i++)
		{
			VkImageViewCreateInfo info;
			info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			info.pNext = nullptr;
			info.flags = 0;
			info.image = pImages[i];
			info.viewType = VK_IMAGE_VIEW_TYPE_2D;
			info.format = swapchainFormat;
			info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			info.subresourceRange.baseArrayLayer = 0;
			info.subresourceRange.baseMipLevel = 0;
			info.subresourceRange.levelCount = 1;
			info.subresourceRange.layerCount = 1;
			info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

			VkResult result = vkCreateImageView(device, &info, nullptr, &pImageViews[i]);
			if (result != VK_SUCCESS)
			{
				for (U32 j = i; j > 0; j--)
					vkDestroyImageView(device, pImageViews[j], nullptr);
				free(pImageViews);
				free(pFramebuffers);
				return result;
			}

		}
		
		for (U32 i = 0; i < imageCount; i++)
		{
			VkFramebufferCreateInfo info;
			info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			info.pNext = nullptr;
			info.flags = 0;
			info.renderPass = swapchainRenderPass;
			info.attachmentCount = 1;
			info.pAttachments = &pImageViews[i];
			info.width = swapchainExtent.width;
			info.height = swapchainExtent.height;
			info.layers = 1;

			VkResult result = vkCreateFramebuffer(device, &info, nullptr, &pFramebuffers[i]);
			if (result != VK_SUCCESS)
			{
				for (U32 j = i; j > 0; j--)
					vkDestroyFramebuffer(device, pFramebuffers[j], nullptr);
				free(pImageViews);
				free(pFramebuffers);
				return result;
			}
		}
		
		*pImageCount = imageCount;
		*ppImageViews = pImageViews;
		*ppFramebuffers = pFramebuffers;

		free(pImages);
		return VK_SUCCESS;
	}

	VkResult CreateVulkanSwapchainRenderPass(VkDevice device, VkFormat swapchainFormat, VkRenderPass* pRenderPass)
	{
		VkAttachmentDescription colorDesc;
		colorDesc.flags = 0;
		colorDesc.format = swapchainFormat;
		colorDesc.samples = VK_SAMPLE_COUNT_1_BIT;
		colorDesc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorDesc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorDesc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorDesc.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachRef;
		colorAttachRef.attachment = 0;
		colorAttachRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription colorSubpass;
		colorSubpass.flags = 0;
		colorSubpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		colorSubpass.inputAttachmentCount = 0;
		colorSubpass.pInputAttachments = nullptr;
		colorSubpass.colorAttachmentCount = 1;
		colorSubpass.pColorAttachments = &colorAttachRef;
		colorSubpass.pResolveAttachments = nullptr;
		colorSubpass.pDepthStencilAttachment = nullptr;
		colorSubpass.preserveAttachmentCount = 0;
		colorSubpass.pPreserveAttachments = nullptr;

		//VkSubpassDependency colorDependency;

		VkRenderPassCreateInfo info;
		info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = 0;
		info.attachmentCount = 1;
		info.pAttachments = &colorDesc;
		info.subpassCount = 1;
		info.pSubpasses = &colorSubpass;
		info.dependencyCount = 0;
		info.pDependencies = nullptr;

		return vkCreateRenderPass(device, &info, nullptr, pRenderPass);
	}
	
	VkResult AllocateVulkanCommandBuffer(VkDevice device, VkCommandPool commandPool, VkCommandBuffer* pBuffer)
	{
		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;
		
		return vkAllocateCommandBuffers(device, &allocInfo, pBuffer);
	}

	VkResult BeginVulkanCommandBuffer(VkCommandBuffer cmdBuffer)
	{
		VkCommandBufferBeginInfo begin;
		begin.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		begin.pNext = nullptr;
		begin.flags = 0;
		begin.pInheritanceInfo = nullptr;

		return vkBeginCommandBuffer(cmdBuffer, &begin);
	}

	void BeginVulkanRenderPass(VkCommandBuffer cmdBuffer, VkRenderPass renderPass, VkFramebuffer framebuffer, VkExtent2D areaExtent)
	{
		VkRenderPassBeginInfo info;
		info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		info.pNext = nullptr;
		info.renderPass = renderPass;
		info.framebuffer = framebuffer;
		info.renderArea.extent = areaExtent;
		info.renderArea.offset = { 0, 0 };
		VkClearValue clearValue = {};
		clearValue.color.float32[0] = 0.0f;
		clearValue.color.float32[1] = 0.0f;
		clearValue.color.float32[2] = 0.5f;
		clearValue.color.float32[3] = 1.0f;
		info.pClearValues = &clearValue;
		info.clearValueCount = 1;
		vkCmdBeginRenderPass(cmdBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
	}

	void VulkanImageMamoryBarrier(
		VkCommandBuffer cmdBuffer,
		VkPipelineStageFlags srcStageMask,
		VkPipelineStageFlags dstStageMask,
		const VkImageMemoryBarrier* pImageBarrier
	)
	{
		vkCmdPipelineBarrier(cmdBuffer, srcStageMask, dstStageMask, VK_DEPENDENCY_BY_REGION_BIT, 0, nullptr, 0, nullptr, 1, pImageBarrier);
	}

	VkResult SumbitToVulkanQueue(
		VkQueue queue,
		VkCommandBuffer cmdBuffer,
		const VulkanQueueSubmitSyncObjects* pSyncObj,
		VkFence fence
	)
	{
		VkSubmitInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		info.commandBufferCount = 1;
		info.pCommandBuffers = &cmdBuffer;
		if (pSyncObj)
		{
			info.pWaitDstStageMask = pSyncObj->pWaitDstStageFlags;
			info.pWaitSemaphores = pSyncObj->pWaitSemaphores;
			info.waitSemaphoreCount = pSyncObj->waitSemaphoreCount;
			info.pSignalSemaphores = pSyncObj->pSignalSemaphores;
			info.signalSemaphoreCount = pSyncObj->signalSemaphoreCount;
		}
		
		return vkQueueSubmit(queue, 1, &info, fence);
	}

	VkResult PresentVulkanSwapchain(
		VkQueue queue,
		VkSwapchainKHR swapchain,
		U32 imageIndex,
		VkSemaphore* pWaits,
		U32 waitCount
	)
	{
		VkPresentInfoKHR presentInfo;
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.pNext = nullptr;
		presentInfo.swapchainCount = 1;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr;
		presentInfo.pSwapchains = &swapchain;

		presentInfo.waitSemaphoreCount = waitCount;
		presentInfo.pWaitSemaphores = pWaits;
		return vkQueuePresentKHR(queue, &presentInfo);
	}

	VkResult CreateVulkanFence(VkDevice device, VkFenceCreateFlags flags, VkFence* pFence)
	{
		VkFenceCreateInfo fenceInfo;
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = flags;
		fenceInfo.pNext = nullptr;
		return vkCreateFence(device, &fenceInfo, nullptr, pFence);
	}

	VkResult CreateVulkanSemaphore(VkDevice device, VkSemaphore* pSemaphore)
	{
		VkSemaphoreCreateInfo info;
		info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = 0;

		return vkCreateSemaphore(device, &info, nullptr, pSemaphore);
	}

	VkResult CreateVulkanBuffer(VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage, VkBuffer* pBuffer)
	{
		VkBufferCreateInfo info;
		info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = 0;
		info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		info.pQueueFamilyIndices = nullptr;
		info.queueFamilyIndexCount = 0;
		info.size = size;
		info.usage = usage;
		
		return vkCreateBuffer(device, &info, nullptr, pBuffer);
	}

	VkResult CreateVulkanImageView(VkDevice device, VkImage image, VkFormat format, VkImageSubresourceRange subresourceRange, VkImageView* pView)
	{
		VkImageViewCreateInfo info;
		info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = 0;
		info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		info.format = format;
		info.image = image;
		info.subresourceRange = subresourceRange;
		info.viewType = VK_IMAGE_VIEW_TYPE_2D;

		return vkCreateImageView(device, &info, nullptr, pView);
	}

	VkResult CreateVulkanShader(VkDevice device, U32* pData, U64 size, VkShaderModule* pShader)
	{
		VkShaderModuleCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		info.codeSize = size;
		info.pCode = pData;

		return vkCreateShaderModule(device, &info, nullptr, pShader);;
	}

	VkResult CreateVulkanDescriptorPool(VkDevice device, U32 maxSets, VkDescriptorPoolSize* pSizes, U32 sizeCount, VkDescriptorPool* pPool)
	{
		VkDescriptorPoolCreateInfo info;
		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		info.flags = 0;
		info.pNext = nullptr;
		info.pPoolSizes = pSizes;
		info.poolSizeCount = sizeCount;
		info.maxSets = maxSets;
		return vkCreateDescriptorPool(device, &info, nullptr, pPool);
	}

	VkResult AllocateVulkanDescriptorSet(VkDevice device, VkDescriptorPool pool, VkDescriptorSetLayout layout, VkDescriptorSet* pSet)
	{
		VkDescriptorSetAllocateInfo info;
		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		info.pNext = nullptr;
		info.pSetLayouts = &layout;
		info.descriptorPool = pool;
		info.descriptorSetCount = 1;

		VkResult result = vkAllocateDescriptorSets(device, &info, pSet);
		if (result != VK_SUCCESS)
			return result;

		return result;
	}
}