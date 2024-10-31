#pragma once
#include <PrimitiveTypes.h>
#ifdef MT_PLATFORM_WINDOWS
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#include "Window.h"
#include <volk.h>
#include <PoolAllocator.h>

namespace MarkTech
{
	enum VulkanContextCreateFlagBits
	{
		VULKAN_CONTEXT_FLAGS_DEBUG_MESSAGES = 0x1
	};

	enum VulkanResultCode
	{
		VRC_SUCCESS,
		VRC_FAILED
	};

	typedef U32 VulkanContextCreateFlags;

	struct VulkanApplicationInfo
	{
		const char* pName;
		U32 verMajor;
		U32 verMinor;
		U32 verPatch;
	};

	struct VulkanContextInfo
	{
		VulkanApplicationInfo* pAppInfo;
		VulkanContextCreateFlags flags;
	};

	struct QueueFamilyIndices
	{
		U32 graphicsQueue;
	};

	struct VulkanContext
	{
		VkInstance instance;
		VkDevice device;
		VkDebugUtilsMessengerEXT messenger;
	};

	struct VulkanSwapchain
	{
		VkSwapchainKHR swapchain;
	};

	VulkanResultCode CreateVulkanContext(const VulkanContextInfo& info, VulkanContext** ppContext);
	void DestroyVulkanContext(VulkanContext* pContext);
}