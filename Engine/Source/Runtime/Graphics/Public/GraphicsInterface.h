#pragma once
#include <PrimitiveTypes.h>
#include <volk.h>

#define MT_MAX_GRAPHICS_DEVICES 8
#define MT_MAX_QUEUE_FAMILIES 8

namespace MarkTech
{
	struct VulkanGraphicsContext
	{
		VkInstance m_vkInstance;
	};

	struct VulkanPhysicalDevice
	{
		VkPhysicalDevice m_vkPhysicalDevice;
	};

	struct VulkanGraphicsDevice
	{
		VkDevice m_vkDevice;
	};

	struct VulkanQueueFamiles
	{
		U32 graphicsFamily;
	};

	enum GraphicsExtensions : U32
	{
		EXT_DEBUG = 0x02
	};

	struct ContextSettings
	{
		U32 majorVersion;
		U32 minorVersion;
		U32 patchVersion;
		U32 extensionBitflags;
		char* pAppName;
	};

	VulkanGraphicsContext CreateGraphicsContext(ContextSettings settings);
	bool SetupDebugCallback(VulkanGraphicsContext* pContext);
	void DestroyDebugCallback(VulkanGraphicsContext* pContext);
	void DestroyGraphicsContext(VulkanGraphicsContext* pContext);
	VulkanPhysicalDevice ChooseBestGraphicsDevice(VulkanGraphicsContext* pContext);
	VulkanGraphicsDevice CreateGraphicsDevice(VulkanPhysicalDevice* pPhysicalDevice);
	void DestroyGraphicsDevice(VulkanGraphicsDevice* pDevice);
	VulkanQueueFamiles FindQueueFamilies(VulkanPhysicalDevice* pPhysicalDevice);
}