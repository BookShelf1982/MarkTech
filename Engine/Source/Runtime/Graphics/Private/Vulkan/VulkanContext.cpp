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

	VulkanResultCode CreateVulkanContext(const VulkanContextInfo& info, VulkanContext** ppContext)
	{
		volkInitialize();
		CreatePoolAllocator(gObjAlloc, sizeof(VulkanContext), 16);
		VulkanContext* pVulkanContext = (VulkanContext*)AllocFromPool(gObjAlloc);
		*ppContext = pVulkanContext;
		pVulkanContext->instance = VK_NULL_HANDLE;
		pVulkanContext->messenger = VK_NULL_HANDLE;

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

		Array<const char*> extensionNames;
		ReserveArray(extensionNames, 8, nullptr);

		InsertArrayItem(extensionNames, VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME);

		if (info.flags & VULKAN_CONTEXT_FLAGS_DEBUG_MESSAGES)
		{
			InsertArrayItem(extensionNames, VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		instanceInfo.enabledExtensionCount = extensionNames.size;
		instanceInfo.ppEnabledExtensionNames = extensionNames.size > 0 ? extensionNames.pArray : nullptr;

		instanceInfo.enabledLayerCount = 0;
		instanceInfo.ppEnabledLayerNames = nullptr;

		if (vkCreateInstance(&instanceInfo, nullptr, &pVulkanContext->instance) != VK_SUCCESS)
		{
			volkFinalize();
			FreeToPool(gObjAlloc, pVulkanContext);
			FreePoolAllocator(gObjAlloc);
			return VRC_FAILED;
		}

		DestroyArray(extensionNames);
		volkLoadInstance(pVulkanContext->instance);

		VkDebugUtilsMessengerCreateInfoEXT debugInfo = {};
		debugInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;;
		debugInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;;
		debugInfo.pfnUserCallback = DebugMessagengerCallback;
		if (vkCreateDebugUtilsMessengerEXT(pVulkanContext->instance, &debugInfo, nullptr, &pVulkanContext->messenger) != VK_SUCCESS)
		{
			DestroyVulkanContext(pVulkanContext);
			return VRC_FAILED;
		}



		return VRC_SUCCESS;
	}

	void DestroyVulkanContext(VulkanContext* pContext)
	{
		if (pContext->messenger)
			vkDestroyDebugUtilsMessengerEXT(pContext->instance, pContext->messenger, nullptr);

		vkDestroyInstance(pContext->instance, nullptr);
		volkFinalize();
		FreeToPool(gObjAlloc, pContext);
		FreePoolAllocator(gObjAlloc);
	}
}