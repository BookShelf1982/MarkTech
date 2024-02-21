#pragma once
#include "Renderer.h"

#ifdef MT_PLATFORM_WINDOWS
#define VK_USE_PLATFORM_WIN32_KHR
#include "WinWindow.h"
#endif

#ifdef DEBUG
#define USE_VALIDATION_LAYERS
#endif

#include <optional>
#include <vector>

#include <Volk\volk.h>
#include "VmaUsage.h"

#define vkSUCCEDED(result) result == VK_SUCCESS
#define vkFAILED(result) result != VK_SUCCESS
#define MAX_FRAMES_IN_FLIGHT 2

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

class CVulkanBuffer : public IBuffer
{
public:
	CVulkanBuffer(VkDevice device, VmaAllocator allocator, VkCommandPool commandPool, VkQueue queue, void* data, size_t dataSize);
	~CVulkanBuffer();
	VkBuffer GetBuffer() { return m_vkBuffer; }
	virtual void ReleaseBuffer() override;
private:
	VkBuffer m_vkBuffer;
	VkDevice m_vkDeviceRef;
	VmaAllocation m_vmaAllocation;
	VmaAllocator m_vmaAllocatorRef;
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

class CVulkanConstantBuffer : public IConstantBuffer
{
public:
	CVulkanConstantBuffer(VkDevice device, VmaAllocator allocator, VkDescriptorPool pool, EUsageType usage, size_t bufferSize);
	~CVulkanConstantBuffer();
	void UpdateBuffer(void* data, size_t size, size_t index);
	virtual void ReleaseBuffer() override;
	VkDescriptorSetLayout GetLayout() { return m_vkUniformLayout; }
	VkDescriptorSet* GetDescriptorSetPtr(size_t index) { return &m_vkDescriptorSets[index]; }
private:
	VkDevice m_vkDeviceRef;
	VmaAllocator m_vmaAllocatorRef;
	VkDescriptorSetLayout m_vkUniformLayout;

	std::vector<VkDescriptorSet> m_vkDescriptorSets;
	std::vector<VkBuffer> m_vkUniformBuffers;
	std::vector<VmaAllocation> m_vmaUniformAllocations;
	std::vector<void*> m_pUniformBuffersMapped;
};

class CVulkanPipelineObject : public IPipelineObject
{
public:
	CVulkanPipelineObject(
		VkDevice device, 
		VkShaderModule vertShader, 
		VkShaderModule fragShader, 
		VkRenderPass renderPass, 
		VkDescriptorSetLayout* setLayouts, 
		size_t setLayoutCounts);

	~CVulkanPipelineObject();
	virtual void Release() override;
	VkPipeline GetPipeline() { return m_vkPipeline; }
	VkPipelineLayout GetLayout() { return m_vkPipelineLayout; }
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

	virtual IBuffer* CreateBuffer(char* data, size_t dataSize) override;

	virtual IConstantBuffer* CreateConstantBuffer(size_t dataSize, EUsageType usage) override;
	virtual void UpdateConstantBuffer(IConstantBuffer* constantBuffer, void* data, size_t size) override;

	virtual IPipelineObject* CreatePipeline(IShader* vertexShader, IShader* fragmentShader, IConstantBuffer** constantBuffers, size_t constantBufferCount) override;

	virtual void BeginCommandRecording() override;
	virtual void EndCommandRecording() override;
	virtual void SubmitCommandRecording() override;
	virtual void BindPipelineObject(IPipelineObject* pipeline) override;
	virtual void BindVertexBuffer(IBuffer* buffer, size_t offset) override;
	virtual void BindIndexBuffer(IBuffer* buffer, size_t offset) override;
	virtual void BindConstantBuffer(IConstantBuffer* buffer) override;
	virtual void SetViewportRect(MViewport viewport) override;
	virtual void SetScissorRect(MRect rect) override;
	virtual void DrawVertices(uint32_t numVerts) override;
	virtual void DrawIndices(uint32_t numInds) override;

	virtual void WaitForPreviousFrame() override;
	virtual void WaitForDeviceToIdle() override;

	virtual void AquireNextSwapChainImage() override;
	virtual void Present() override;
private:
	// -- General stuff -- //
	VkInstance m_vkInstance;
	VkPhysicalDevice m_vkPhysicalDevice;
	VkDevice m_vkDevice;
	VkQueue m_vkGraphicsRenderQueue;
	VkQueue m_vkPresentQueue;
	VmaAllocator m_vmaAllocator; // Vulkan Memory Allocator
	CVulkanPipelineObject* m_pCurrentBoundPipeline; // The current bound pipeline object

	// -- Swap chain stuff -- //
	uint32_t m_nImageIndex;
	uint32_t m_nCurrentFrame;
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

	// -- depth stencil stuff -- //
	VkImage m_vkDepthStencilImage;
	VkImageView m_vkDepthStencilImageView;
	VmaAllocation m_vmaDepthStencilAllocation;
	VkFormat m_vkDepthStencilFormat;

	// -- Command buffers -- //
	VkCommandPool m_vkCommandPool;
	VkCommandPool m_vkTransferCommandPool;
	std::vector<VkCommandBuffer> m_vkCommandBuffer;

	// -- Sync objects -- //
	std::vector<VkSemaphore> m_vkSwapChainBufferAvailable;
	std::vector<VkSemaphore> m_vkFinishedRendering;
	std::vector<VkFence> m_vkInFlightFence;

	// -- Descriptor sets -- //
	VkDescriptorPool m_vkDescriptorPool;


	// -- Private Helper Funcitons -- //
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char*>& extensions);
	bool IsDeviceSuitable(VkPhysicalDevice device, const std::vector<const char*>& extensions);
	MSwapChainDetails GetSwapChainDetails(VkPhysicalDevice device);
	MQueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
	VkSurfaceFormatKHR ChooseSwapChainFormat(const std::vector<VkSurfaceFormatKHR>& formats);
	VkPresentModeKHR ChooseSwapChainPresentMode(const std::vector<VkPresentModeKHR>& modes);
	bool CreateSwapChain(VkDevice device, VkSwapchainKHR& swpachain, MQueueFamilyIndices indices, IWindow* pWindow);
	VkExtent2D ChooseSwapChainExtent2D(const VkSurfaceCapabilitiesKHR& capabilities, IWindow* pWindow);
	VkShaderModule CreateShader(const std::vector<char>& buffer);
	VkFormat FindSupportedFormat(const std::vector<VkFormat>& canidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkFormat FindDepthFormat();
	void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	bool HasStencilComponent(VkFormat format);
	void CreateImageViews();
	bool CreateFrameBuffers();


#ifdef MT_PLATFORM_WINDOWS
	HMODULE VulkanLibrary;
#endif

#ifdef USE_VALIDATION_LAYERS
	VkDebugUtilsMessengerEXT m_vkDebugMessenger;

	void CreateDebugMessenger(VkInstance instance, VkDebugUtilsMessengerEXT* pDebugMessenger);
	void DestroyDebugMessenger(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger);
#endif
};