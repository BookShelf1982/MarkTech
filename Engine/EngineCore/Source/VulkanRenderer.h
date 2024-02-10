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

class CVulkanShader : public IShader
{
public:
	CVulkanShader(VkDevice device, char* data, size_t dataSize);
	~CVulkanShader();
	virtual void ReleaseShader() override;
	VkShaderModule GetShader() { return m_vkShader; }
private:
	VkShaderModule m_vkShader;
	VkDevice m_vkDeviceRef;
};

class CVulkanPipelineObject : public IPipelineObject
{
public:
	CVulkanPipelineObject(VkDevice device, VkShaderModule vertShader, VkShaderModule fragShader, VkRenderPass renderPass);
	~CVulkanPipelineObject();
	virtual void Release() override;
	VkPipeline GetPipeline() { return m_vkPipeline; }
private:
	VkPipeline m_vkPipeline;
	VkPipelineLayout m_vkPipelineLayout;
	VkDevice m_vkDeviceRef;
};

class CVulkanRenderer : public IRenderer
{
public:
	CVulkanRenderer();
	~CVulkanRenderer();

	virtual	bool InitRenderer(IWindow* window) override;
	virtual	void ShutdownRenderer() override;

	virtual IShader* CreateShader(char* data, size_t dataSize) override;

	virtual IPipelineObject* CreatePipeline(IShader* vertexShader, IShader* fragmentShader) override;

	virtual void BeginCommandRecording() override;
	virtual void EndCommandRecording() override;
	virtual void BindPipelineObject(IPipelineObject* pipeline) override;
	virtual void SetViewportRect(MViewport viewport) override;
	virtual void SetScissorRect(MRect rect) override;
	virtual void DrawVertices(uint32_t numVerts) override;
private:
	// -- General stuff -- //
	VkInstance m_vkInstance;
	VkPhysicalDevice m_vkPhysicalDevice;
	VkDevice m_vkDevice;
	VkQueue m_vkGraphicsRenderQueue;
	VkQueue m_vkPresentQueue;

	// -- Swap chain stuff -- //
	uint32_t m_nImageIndex;
	VkSurfaceKHR m_vkWindowSurface;
	VkSwapchainKHR m_vkSwapchain;
	VkImage* m_pvkSwapchainImages;
	VkImageView* m_pvkSwapchainImageViews;
	VkFramebuffer* m_pvkSwapchainFrameBuffers;
	size_t m_nNumSwapchainImages;
	size_t m_nNumSwapchainImageViews;
	size_t m_nNumSwapchainFrameBuffers;
	VkFormat m_vkSwapchainImageFormat;
	VkExtent2D m_vkSwapchainExtent;

	VkRenderPass m_vkRenderPass;

	// -- Command buffers -- //
	VkCommandPool m_vkCommandPool;
	VkCommandBuffer m_vkCommandBuffer;

	// -- Private Helper Funcitons -- //
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char*>& extensions);
	bool IsDeviceSuitable(VkPhysicalDevice device, const std::vector<const char*>& extensions);
	MSwapChainDetails GetSwapChainDetails(VkPhysicalDevice device);
	MQueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
	VkSurfaceFormatKHR ChooseSwapChainFormat(const std::vector<VkSurfaceFormatKHR>& formats);
	VkPresentModeKHR ChooseSwapChainPresentMode(const std::vector<VkPresentModeKHR> modes);
	bool CreateSwapChain(VkDevice device, VkSwapchainKHR& swpachain, MQueueFamilyIndices indices, IWindow* pWindow);
	VkExtent2D ChooseSwapChainExtent2D(const VkSurfaceCapabilitiesKHR& capabilities, IWindow* pWindow);
	VkShaderModule CreateShader(const std::vector<char>& buffer);
	void CreateImageViews();
	bool CreateFrameBuffers();
	static std::vector<char> ReadFromFile(const char* filename);

#ifdef DEBUG
	VkDebugUtilsMessengerEXT m_vkDebugMessenger;

	void CreateDebugMessenger(VkInstance instance, VkDebugUtilsMessengerEXT* pDebugMessenger);
	void DestroyDebugMessenger(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger);
#endif
};