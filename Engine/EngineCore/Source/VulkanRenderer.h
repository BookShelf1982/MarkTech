#pragma once
#include "Renderer.h"
#ifdef MT_PLATFORM_WINDOWS
#define VK_USE_PLATFORM_WIN32_KHR
#include "WinWindow.h"
#endif
#include <vulkan\vulkan.h>
#include <optional>
#include <vector>

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
	std::optional<uint32_t> presentFamily;

	bool IsComplete()
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct MSwapChainDetails
{
	VkSurfaceCapabilitiesKHR surfaceCapabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
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
	VkQueue m_vkGraphicsRenderQueue;
	VkQueue m_vkPresentQueue;
	VkSurfaceKHR m_vkWindowSurface;
	VkSwapchainKHR m_vkSwapchain;

	std::vector<VkImage> m_vkSwapchainImages;
	std::vector<VkImageView> m_vkSwapchainImageViews;
	VkFormat m_vkSwapchainImageFormat;
	VkExtent2D m_vkSwapchainExtent;

#ifdef DEBUG
	VkDebugUtilsMessengerEXT m_vkDebugMessenger;

	void CreateDebugMessenger(VkInstance instance, VkDebugUtilsMessengerEXT* pDebugMessenger);
	void DestroyDebugMessenger(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger);
#endif
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char*>& extensions);
	bool IsDeviceSuitable(VkPhysicalDevice device, const std::vector<const char*>& extensions);
	MSwapChainDetails GetSwapChainDetails(VkPhysicalDevice device);
	MQueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
	VkSurfaceFormatKHR ChooseSwapChainFormat(const std::vector<VkSurfaceFormatKHR>& formats);
	VkPresentModeKHR ChooseSwapChainPresentMode(const std::vector<VkPresentModeKHR> modes);
	bool CreateSwapChain(VkDevice device, VkSwapchainKHR* swpachain, MQueueFamilyIndices indices, IWindow* pWindow);
	VkExtent2D ChooseSwapChainExtent2D(const VkSurfaceCapabilitiesKHR& capabilities, IWindow* pWindow);
	void CreateImageViews();
};