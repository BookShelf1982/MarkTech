#pragma once
#include <PrimitiveTypes.h>
#ifdef MT_PLATFORM_WINDOWS
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#include "Window.h"
#include <volk.h>

namespace MarkTech
{
#define MT_MAX_SWAPCHAIN_IMAGES 2
	struct SwapchainDetails
	{
		VkSurfaceCapabilitiesKHR caps;
		VkSurfaceFormatKHR formats[32];
		VkPresentModeKHR modes[6];
	};

	struct QueueFamilyIndices
	{
		U32 graphicsQueue;
	};

	struct GraphicsContext
	{
		VkInstance instance;
		VkDevice device;
		VkPhysicalDevice physicalDevice;
		QueueFamilyIndices indices;
		VkDebugUtilsMessengerEXT messenger;
		VkQueue graphicsQueue;
	};

	struct Swapchain
	{
		VkSurfaceKHR windowSurface;
		VkSwapchainKHR swapchain;
		VkImageView swapchainImageViews[MT_MAX_SWAPCHAIN_IMAGES];
	};

	struct AppInfo
	{
		const char* pAppName;
		U16 majorVersion;
		U16 minorVersion;
		U16 patchVersion;
	};

	enum GraphicsContextFlags : U8
	{
		GRAPHICS_CONTEXT_FLAGS_USE_WINDOW = 0x02,
		GRAPHICS_CONTEXT_FLAGS_DEBUG_MESSAGES = 0x04
	};

	struct GraphicsContextCreateInfo
	{
		AppInfo appInfo;
		U8 flags;
	};

	GraphicsContext CreateGraphicsContext(const GraphicsContextCreateInfo* pInfo);
	void DestroyGraphicsContext(GraphicsContext* pContext);

	Swapchain CreateSwapchain(const GraphicsContext* pContext, const Window* pWindow);
	void DestroySwapchain(const GraphicsContext* pContext, Swapchain* pSwapchain);
}