#pragma once
#include <Version.h>
#include <Array.h>
#include <stdio.h>
#ifdef MT_PLATFORM_WINDOWS
#include <Windows.h>
#define VK_USE_PLATFORM_WIN32_KHR
#endif

#include <volk.h>

namespace MarkTech
{
	VkBool32 DebugMessagengerCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageTypes,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData
	);

	VkResult CreateInstance(
		const Array<const char*> layers,
		const Array<const char*> extensions,
		VkInstance* pInstance
	);

	VkResult CreateDebugMessenger(VkInstance instance, VkDebugUtilsMessengerEXT* pMessenger);
	
	VkPhysicalDevice ChooseBestDevice(VkInstance instance);

	void GetQueueFamilyIndices(VkPhysicalDevice physicalDevice, U32* pGraphicsIndex);

	VkResult CreateLogicalDevice(VkPhysicalDevice physicalDevice, VkDevice* pDevice, U32 graphicsQueueIndex);
	
#ifdef MT_PLATFORM_WINDOWS
	VkResult CreateSurfaceWin32(VkInstance instance, HWND window, VkSurfaceKHR* pSurface);
#endif

	VkSurfaceFormatKHR GetBestSurfaceFormat(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceFormatKHR desiredFormat);

	VkResult CreateVkSwapchain(
		VkDevice device,
		VkPhysicalDevice physicalDevice,
		VkSurfaceKHR surface,
		VkPresentModeKHR presentMode,
		VkSwapchainKHR oldSwapchain,
		VkSwapchainKHR* pSwapchain,
		VkFormat* pFormat,
		VkExtent2D* pExtent
	);

	VkResult CreateSwapchainRenderPass(VkDevice device, VkFormat swapchainFormat, VkRenderPass* pRenderPass);

	VkResult CreateSwapchainImageViews(
		VkDevice device,
		VkSwapchainKHR swapchain,
		U32* pImageCount,
		VkFormat imageFormat,
		Array<VkImageView>& imageViews
	);
	
	VkResult CreateSwapchainFrameBuffers(
		VkDevice device,
		VkExtent2D swapchainExtent,
		VkRenderPass renderPass,
		Array<VkImageView>& imageViews,
		Array<VkFramebuffer>& framebuffers
	);

	struct DescriptorSetLayoutBuilder
	{
		U32 bindingIndex;
		VkDescriptorSetLayoutCreateInfo info;
		Array<VkDescriptorSetLayoutBinding> binding;
	};

	void BeginSetLayoutBuilder(DescriptorSetLayoutBuilder& builder);
	void StartNewBinding(DescriptorSetLayoutBuilder& builder);
	void AddDescriptor(DescriptorSetLayoutBuilder& buidler, VkDescriptorType type, U32 count, VkShaderStageFlags stageFlags);
	void DestroySetLayoutBuilder(DescriptorSetLayoutBuilder& builder);
	VkResult BuildSetLayoutBuilder(DescriptorSetLayoutBuilder& builder, VkDevice device, VkDescriptorSetLayout* pLayout);
}