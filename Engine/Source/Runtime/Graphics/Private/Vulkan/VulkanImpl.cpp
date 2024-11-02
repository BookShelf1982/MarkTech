#include "VulkanImpl.h"
#include <PoolAllocator.h>
#include <Array.h>
#include <Version.h>

#include <stdio.h>

#define ENGINE_NAME "MarkTech"

namespace MarkTech
{
	PoolAllocator gObjAlloc;

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

	static VkResult CreateInstance(const VulkanContextInfo& info, VkInstance* pInstance)
	{
		VkApplicationInfo appInfo;
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pNext = nullptr;
		appInfo.apiVersion = VK_API_VERSION_1_2;
		appInfo.applicationVersion = VK_MAKE_VERSION(info.pAppInfo->verMajor, info.pAppInfo->verMinor, info.pAppInfo->verPatch);
		appInfo.pApplicationName = info.pAppInfo->pName;
		appInfo.engineVersion = VK_MAKE_VERSION(MT_MAJOR_VERSION, MT_MINOR_VERSION, MT_PATCH_VERSION);
		appInfo.pEngineName = ENGINE_NAME;

		VkInstanceCreateInfo instanceInfo;
		instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceInfo.pNext = nullptr;
		instanceInfo.flags = 0;
		instanceInfo.pApplicationInfo = &appInfo;

		Array<const char*> layerNames;
		ReserveArray(layerNames, 1, nullptr);

		Array<const char*> extensionNames;
		ReserveArray(extensionNames, 8, nullptr);
		InsertArrayItem(extensionNames, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
		InsertArrayItem(extensionNames, VK_KHR_SURFACE_EXTENSION_NAME);
#ifdef MT_PLATFORM_WINDOWS
		InsertArrayItem(extensionNames, VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#endif
		if (info.flags & VULKAN_CONTEXT_FLAGS_DEBUG_MESSAGES)
		{
			InsertArrayItem(extensionNames, VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
			InsertArrayItem(layerNames, "VK_LAYER_KHRONOS_validation");
		}

		instanceInfo.enabledExtensionCount = extensionNames.size;
		instanceInfo.ppEnabledExtensionNames = extensionNames.size > 0 ? extensionNames.pArray : nullptr;

		instanceInfo.enabledLayerCount = layerNames.size;
		instanceInfo.ppEnabledLayerNames = layerNames.size > 0 ? layerNames.pArray : nullptr;

		VkResult result = vkCreateInstance(&instanceInfo, nullptr, pInstance);
		if (result != VK_SUCCESS)
			return result;

		DestroyArray(extensionNames);
		DestroyArray(layerNames);
		volkLoadInstance(*pInstance);

		return VK_SUCCESS;
	}

	static VkResult CreateDebugMessenger(VkInstance instance, VkDebugUtilsMessengerEXT* pMessenger)
	{
		VkDebugUtilsMessengerCreateInfoEXT debugInfo = {};
		debugInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;;
		debugInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;;
		debugInfo.pfnUserCallback = DebugMessagengerCallback;

		return vkCreateDebugUtilsMessengerEXT(instance, &debugInfo, nullptr, pMessenger);
	}

	static VkPhysicalDevice ChooseBestDevice(VkInstance instance)
	{
		U32 deviceCount;
		Array<VkPhysicalDevice> physicalDevices;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
		ReserveArray(physicalDevices, deviceCount, nullptr);
		vkEnumeratePhysicalDevices(instance, &deviceCount, physicalDevices.pArray);

		U32 bestDeviceIndex = 0;
		U32 prevScore = 0;
		for (U32 i = 0; i < deviceCount; i++)
		{
			U32 score = 0;
			VkPhysicalDeviceProperties deviceProps = {};
			vkGetPhysicalDeviceProperties(physicalDevices.pArray[i], &deviceProps);
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

		VkPhysicalDevice bestDevice = physicalDevices.pArray[bestDeviceIndex];
		DestroyArray(physicalDevices);
		return bestDevice;
	}
	
	static void GetQueueFamilyIndices(VkPhysicalDevice physicalDevice, QueueFamilyIndices& indices)
	{
		U32 familyCount = 0;
		Array<VkQueueFamilyProperties> queueFamiles;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &familyCount, nullptr);
		ReserveArray(queueFamiles, familyCount, nullptr);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &familyCount, queueFamiles.pArray);

		U32 queuesLeft = 0;
		for (U32 i = 0; i < familyCount; i++)
		{
			if (queueFamiles.pArray[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				indices.graphicsQueue = i;
				break;
			}
		}

		DestroyArray(queueFamiles);
	}

	static VkResult CreateLogicalDevice(VkPhysicalDevice physicalDevice, VkDevice* pDevice)
	{
		VkDeviceCreateInfo info;
		info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = 0;
		Array<const char*> extensionNames;
		ReserveArray(extensionNames, 8, nullptr);
		InsertArrayItem(extensionNames, VK_KHR_SWAPCHAIN_EXTENSION_NAME);

		info.enabledExtensionCount = extensionNames.size;
		info.ppEnabledExtensionNames = extensionNames.size > 0 ? extensionNames.pArray : nullptr;;
		info.enabledLayerCount = 0;
		info.ppEnabledLayerNames = nullptr;

		VkPhysicalDeviceFeatures features = {};
		info.pEnabledFeatures = &features;

		QueueFamilyIndices queueIndices;
		GetQueueFamilyIndices(physicalDevice, queueIndices);

		VkDeviceQueueCreateInfo queueInfo;
		queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueInfo.pNext = nullptr;
		queueInfo.flags = 0;
		float priorities = 1.0f;
		queueInfo.pQueuePriorities = &priorities;
		queueInfo.queueCount = 1;
		queueInfo.queueFamilyIndex = queueIndices.graphicsQueue;

		info.queueCreateInfoCount = 1;
		info.pQueueCreateInfos = &queueInfo;
		
		VkResult result = vkCreateDevice(physicalDevice, &info, nullptr, pDevice);
		
		DestroyArray(extensionNames);
		return result;
	}

	static VkResult CreateSurface(VkInstance instance, const Window& window, VkSurfaceKHR* pSurface)
	{
#ifdef MT_PLATFORM_WINDOWS
		VkWin32SurfaceCreateInfoKHR surfaceInfo;
		surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		surfaceInfo.pNext = nullptr;
		surfaceInfo.flags = 0;
		surfaceInfo.hwnd = window.hWnd;
		surfaceInfo.hinstance = ghApplicationInstance;
		return vkCreateWin32SurfaceKHR(instance, &surfaceInfo, nullptr, pSurface);
#endif
	}

	static VkSurfaceFormatKHR GetBestSurfaceFormat(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceFormatKHR desiredFormat)
	{
		Array<VkSurfaceFormatKHR> surfaceFormats;
		U32 formatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
		ReserveArray(surfaceFormats, formatCount, nullptr);
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, surfaceFormats.pArray);
		VkSurfaceFormatKHR format = surfaceFormats.pArray[0];

		for (U32 i = 0; i < formatCount; i++)
		{
			if (surfaceFormats.pArray[i].format == desiredFormat.format && surfaceFormats.pArray[i].colorSpace == desiredFormat.colorSpace)
			{
				VkSurfaceFormatKHR format = surfaceFormats.pArray[i];
			}
		}
		DestroyArray(surfaceFormats);
		return format;
	}

	static VkResult CreateVkSwapchain(
		VkDevice device,
		VkPhysicalDevice physicalDevice,
		VkSurfaceKHR surface,
		VkPresentModeKHR presentMode,
		VkSwapchainKHR oldSwapchain,
		VkSwapchainKHR* pSwapchain
	)
	{
		VkSurfaceFormatKHR surfaceFormat = GetBestSurfaceFormat(physicalDevice, surface, { VK_FORMAT_B8G8R8A8_UINT, VK_COLORSPACE_SRGB_NONLINEAR_KHR });
		VkSurfaceCapabilitiesKHR surfaceCaps;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCaps);

		VkSwapchainCreateInfoKHR swapchainInfo;
		swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainInfo.pNext = nullptr;
		swapchainInfo.flags = 0;
		swapchainInfo.surface = surface;
		swapchainInfo.minImageCount = surfaceCaps.minImageCount + 1;
		swapchainInfo.imageFormat = surfaceFormat.format;
		swapchainInfo.imageColorSpace = surfaceFormat.colorSpace;
		swapchainInfo.imageExtent = surfaceCaps.currentExtent;
		swapchainInfo.imageArrayLayers = 1;
		swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchainInfo.queueFamilyIndexCount = 0;
		swapchainInfo.pQueueFamilyIndices = nullptr;
		swapchainInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainInfo.presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
		swapchainInfo.clipped = VK_TRUE;
		swapchainInfo.oldSwapchain = oldSwapchain;

		return vkCreateSwapchainKHR(device, &swapchainInfo, nullptr, pSwapchain);
	}

	VulkanResultCode CreateVulkanContext(const VulkanContextInfo& info, VulkanContext** ppContext)
	{
		volkInitialize();
		CreatePoolAllocator(gObjAlloc, sizeof(VulkanContext), 16);
		VulkanContext* pVulkanContext = (VulkanContext*)AllocFromPool(gObjAlloc);
		*ppContext = pVulkanContext;

		if (CreateInstance(info, &pVulkanContext->instance) != VK_SUCCESS)
		{
			volkFinalize();
			FreeToPool(gObjAlloc, pVulkanContext);
			FreePoolAllocator(gObjAlloc);
			return VRC_FAILED;
		}

		if (info.flags & VULKAN_CONTEXT_FLAGS_DEBUG_MESSAGES)
		{
			if (CreateDebugMessenger(pVulkanContext->instance, &pVulkanContext->messenger) != VK_SUCCESS)
			{
				volkFinalize();
				FreeToPool(gObjAlloc, pVulkanContext);
				FreePoolAllocator(gObjAlloc);
				return VRC_FAILED;
			}
		}

		VkPhysicalDevice physicalDevice = ChooseBestDevice(pVulkanContext->instance);
		pVulkanContext->physicalDevice = physicalDevice;

		if (CreateLogicalDevice(physicalDevice, &pVulkanContext->device) != VK_SUCCESS)
		{
			volkFinalize();
			vkDestroyInstance(pVulkanContext->instance, nullptr);
			FreeToPool(gObjAlloc, pVulkanContext);
			FreePoolAllocator(gObjAlloc);
			return VRC_FAILED;
		}

		return VRC_SUCCESS;
	}

	void DestroyVulkanContext(VulkanContext* pContext)
	{
		vkDestroyDevice(pContext->device, nullptr);

		if (pContext->messenger)
			vkDestroyDebugUtilsMessengerEXT(pContext->instance, pContext->messenger, nullptr);

		vkDestroyInstance(pContext->instance, nullptr);
		volkFinalize();
		FreeToPool(gObjAlloc, pContext);
		FreePoolAllocator(gObjAlloc);
	}

	VulkanResultCode CreateVulkanSwapchain(VulkanContext* pContext, const VulkanSwapchainInfo& info, VulkanSwapchain** ppSwapchain)
	{
		VulkanSwapchain* pSwapchain = (VulkanSwapchain*)AllocFromPool(gObjAlloc);
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
			presentMode, oldSwapchain, &pSwapchain->swapchain
		) != VK_SUCCESS)
		{
			vkDestroySurfaceKHR(pContext->instance, pSwapchain->surface, nullptr);
			FreeToPool(gObjAlloc, pSwapchain);
			return VRC_FAILED;
		}

		return VRC_SUCCESS;
	}

	void DestroyVulkanSwapchain(VulkanContext* pContext, VulkanSwapchain* pSwapchain)
	{
		vkDestroySwapchainKHR(pContext->device, pSwapchain->swapchain, nullptr);
		vkDestroySurfaceKHR(pContext->instance, pSwapchain->surface, nullptr);
		FreeToPool(gObjAlloc, pSwapchain);
	}
}