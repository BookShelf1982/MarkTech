#pragma once
#include <PrimitiveTypes.h>
#ifdef MT_PLATFORM_WINDOWS
#include <Windows.h>
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#include <volk.h>

namespace MarkTech
{
	struct QueueFamilyIndices
	{
		U32 graphicsQueue;
		U32 transferQueue;
	};

	struct GraphicsContext
	{
		VkInstance instance;
		VkDevice device;
		QueueFamilyIndices indices;
		VkDebugUtilsMessengerEXT messenger;
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
	void DestroyGraphicsContext(GraphicsContext* pContrxt);
}