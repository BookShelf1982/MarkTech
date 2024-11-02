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

	struct QueueFamilyIndices
	{
		U32 graphicsQueue;
	};

	struct VulkanContextInfo
	{
		VulkanApplicationInfo* pAppInfo;
		VulkanContextCreateFlags flags;
	};

	struct VulkanContext
	{
		VkInstance instance;
		VkDevice device;
		VkPhysicalDevice physicalDevice;
		VkDebugUtilsMessengerEXT messenger;
	};

	VulkanResultCode CreateVulkanContext(const VulkanContextInfo& info, VulkanContext** ppContext);
	void DestroyVulkanContext(VulkanContext* pContext);

	enum VulkanPresentationMode
	{
		VULKAN_PRESENTATION_MODE_FIFO,
		VULKAN_PRESENTATION_MODE_IMMEDIATE
	};

	struct VulkanSwapchain
	{
		VkSurfaceKHR surface;
		VkSwapchainKHR swapchain;
	};

	struct VulkanSwapchainInfo
	{
		Window* pWindow;
		VulkanPresentationMode presentationMode;
		VulkanSwapchain* pOldSwapchain;
	};

	VulkanResultCode CreateVulkanSwapchain(VulkanContext* pContext, const VulkanSwapchainInfo& info, VulkanSwapchain** ppSwapchain);
	void DestroyVulkanSwapchain(VulkanContext* pContext, VulkanSwapchain* pSwapchain);
}