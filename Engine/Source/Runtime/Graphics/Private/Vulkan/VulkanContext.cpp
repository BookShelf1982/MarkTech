#include "VulkanContext.h"
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
		info.enabledExtensionCount = 0;
		info.ppEnabledExtensionNames = nullptr;
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

		return vkCreateDevice(physicalDevice, &info, nullptr, pDevice);
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
}