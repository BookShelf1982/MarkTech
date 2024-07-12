#include "Platform\Vulkan\VulkanGraphicsContext.h"
#include <Version.h>

namespace MarkTech
{
	VulkanGraphisContext::VulkanGraphisContext(Window* window)
		:m_vkInstance(VK_NULL_HANDLE)
	{
		if (volkInitialize() != VK_SUCCESS)
			return;

		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "nullptr"; // TODO: make app name be the game name
		appInfo.applicationVersion = VK_MAKE_VERSION(MT_MAJOR_VERSION, MT_MINOR_VERSION, MT_PATCH_VERSION);
		appInfo.pEngineName = "MarkTech";
		appInfo.engineVersion = VK_MAKE_VERSION(MT_MAJOR_VERSION, MT_MINOR_VERSION, MT_PATCH_VERSION);
		appInfo.apiVersion = VK_API_VERSION_1_2;

		VkInstanceCreateInfo instanceInfo = {};
		instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceInfo.pApplicationInfo = &appInfo;
		instanceInfo.enabledExtensionCount = 0;
		instanceInfo.ppEnabledExtensionNames = nullptr;
		instanceInfo.enabledLayerCount = 0;
		instanceInfo.ppEnabledLayerNames = nullptr;

		VkResult result = vkCreateInstance(&instanceInfo, nullptr, &m_vkInstance);

		volkLoadInstance(m_vkInstance);

		return;
	}

	VulkanGraphisContext::~VulkanGraphisContext()
	{
		Destroy();
	}

	void VulkanGraphisContext::Destroy()
	{
		vkDestroyInstance(m_vkInstance, nullptr);
	}
}