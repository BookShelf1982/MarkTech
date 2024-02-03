#pragma once
#include "Renderer.h"
#include <Windows.h>
#include <vulkan\vulkan.h>
#include <optional>

#define vkSUCCEDED(result) result == VK_SUCCESS
#define vkFAILED(result) result != VK_SUCCESS

void CreateVulkanRenderer(IRenderer** ppRenderer);

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData);

struct MQueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;
	bool IsComplete()
	{
		return graphicsFamily.has_value();
	}
};

class CVulkanRenderer : public IRenderer
{
public:
	virtual	bool InitRenderer(IWindow* window) override;
	virtual	void ShutdownRenderer() override;


private:
	VkInstance m_vkInstance;
	VkPhysicalDevice m_vkPhysicalDevice;
	VkDevice m_vkDevice;
#ifdef DEBUG
	VkDebugUtilsMessengerEXT m_vkDebugMessenger;


	void CreateDebugMessenger(VkInstance instance, VkDebugUtilsMessengerEXT* pDebugMessenger);
	void DestroyDebugMessenger(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger);
	bool IsDeviceSuitable(VkPhysicalDevice device);
	MQueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
#endif


};