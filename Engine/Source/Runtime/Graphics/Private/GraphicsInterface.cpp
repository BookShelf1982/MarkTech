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
		GraphicsContext context = {};

		// Instance Creation
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = pInfo->appInfo.pAppName;
		appInfo.applicationVersion = VK_MAKE_VERSION(pInfo->appInfo.majorVersion, pInfo->appInfo.minorVersion, pInfo->appInfo.patchVersion);
		appInfo.pEngineName = "MarkTech";
		appInfo.engineVersion = VK_MAKE_VERSION(MT_MAJOR_VERSION, MT_MINOR_VERSION, MT_PATCH_VERSION);
		appInfo.apiVersion = VK_VERSION_1_2;

		VkInstanceCreateInfo instanceInfo = {};
		instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceInfo.pApplicationInfo = &appInfo;

		U32 extensionCount = 0;
		char* extensions[8] = {};

		U32 layerCount = 0;
		char* layers[8] = {};

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

		volkInitialize();

		if (vkCreateInstance(&instanceInfo, nullptr, &context.instance) != VK_SUCCESS)
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
		VkPhysicalDevice physicalDevice = ChooseBestDevice(context.instance); // Get best device
		QueueFamilyIndices familyIndices = GetQueueFamilyIndex(physicalDevice);

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

		if (vkCreateDevice(physicalDevice, &deviceInfo, nullptr, &context.device) != VK_SUCCESS)
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
		vkDestroyDebugUtilsMessengerEXT(pContext->instance, pContext->messenger, nullptr);
		vkDestroyInstance(pContext->instance, nullptr);
		volkFinalize();
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

		return swapchain;
	}

	void DestroySwapchain(const GraphicsContext* pContext, Swapchain* pSwapchain)
	{
		vkDestroySwapchainKHR(pContext->device, pSwapchain->swapchain, nullptr);
		vkDestroySurfaceKHR(pContext->instance, pSwapchain->windowSurface, nullptr);
	}
}