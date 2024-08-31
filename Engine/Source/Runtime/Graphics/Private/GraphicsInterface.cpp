#include "GraphicsInterface.h"
#include <Version.h>
#include <Log.h>
#include <stdlib.h>

namespace MarkTech
{
#define MAX_GRAPHICS_DEVICES 4

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

		for (U32 i = 0; i < maxProps; i++)
		{
			if (queueProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				indices.graphicsQueue = i;
			}

			if (queueProps[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
			{
				indices.transferQueue = i;
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

		if (pInfo->flags & GRAPHICS_CONTEXT_FLAGS_USE_WINDOW)
		{
			extensions[extensionCount] = VK_KHR_SURFACE_EXTENSION_NAME;
			extensionCount++;
#ifdef MT_PLATFORM_WINDOWS
			extensions[extensionCount] = VK_KHR_WIN32_SURFACE_EXTENSION_NAME;
			extensionCount++;
#endif
		}

		instanceInfo.ppEnabledExtensionNames = extensions;
		instanceInfo.enabledExtensionCount = extensionCount;

		volkInitialize();

		if (vkCreateInstance(&instanceInfo, nullptr, &context.instance) != VK_SUCCESS)
		{
			return context;
		}

		volkLoadInstance(context.instance);

		// Device Creation
		VkPhysicalDevice physicalDevice = ChooseBestDevice(context.instance); // Get best device

		QueueFamilyIndices familyIndices = GetQueueFamilyIndex(physicalDevice);

		float defaultPrioity = 1.0f;
		VkDeviceQueueCreateInfo graphicsInfo = {};
		graphicsInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		graphicsInfo.pQueuePriorities = &defaultPrioity;
		graphicsInfo.queueCount = 1;
		graphicsInfo.queueFamilyIndex = familyIndices.graphicsQueue;

		VkDeviceQueueCreateInfo transferInfo = {};
		transferInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		transferInfo.pQueuePriorities = &defaultPrioity;
		transferInfo.queueCount = 1;
		transferInfo.queueFamilyIndex = familyIndices.transferQueue;

		VkDeviceQueueCreateInfo queueInfos[2] = { graphicsInfo, transferInfo };

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

		return context;
	}

	void DestroyGraphicsContext(GraphicsContext* pContext)
	{
		vkDestroyDevice(pContext->device, nullptr);
		vkDestroyInstance(pContext->instance, nullptr);
	}
}