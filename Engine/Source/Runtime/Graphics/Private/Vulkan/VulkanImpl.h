#pragma once
#include <PrimitiveTypes.h>
#ifdef MT_PLATFORM_WINDOWS
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#include <Array.h>
#include "Window.h"
#include <volk.h>

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
		VkQueue graphicsQueue;
		VkPhysicalDevice physicalDevice;
		VkDebugUtilsMessengerEXT messenger;
		QueueFamilyIndices queueIndices;
		VkCommandPool commandPool;
		VkFence finishedPreviousFrame;
	};

	VulkanResultCode CreateVulkanContext(const VulkanContextInfo& info, VulkanContext** ppContext);
	void DestroyVulkanContext(VulkanContext* pContext);
	void WaitVulkanDeviceIdle(VulkanContext* pContext);

	struct VulkanFramebuffer
	{
		VkRenderPass renderPass;
		VkFramebuffer framebuffer;
		VkExtent2D extent;
	};

	enum VulkanPresentationMode
	{
		VULKAN_PRESENTATION_MODE_FIFO,
		VULKAN_PRESENTATION_MODE_IMMEDIATE
	};

	struct VulkanSwapchain
	{
		VkSurfaceKHR surface;
		VkSwapchainKHR swapchain;
		Array<VkImage> swapchainImages;
		Array<VkImageView> swapchainImageViews;
		Array<VulkanFramebuffer> swapchainFramebuffers;
		VkRenderPass renderPass;
		VkFormat imageFormat;
		VkExtent2D imageExtent;
		VkFence acquiredImageFence;
		VkSemaphore acquiredImage;
		U32 swapchainImageCount;
		U32 currentImageIndex;
	};

	struct VulkanSwapchainInfo
	{
		Window* pWindow;
		VulkanPresentationMode presentationMode;
		VulkanSwapchain* pOldSwapchain;
	};

	VulkanResultCode CreateVulkanSwapchain(VulkanContext* pContext, const VulkanSwapchainInfo& info, VulkanSwapchain** ppSwapchain);
	void DestroyVulkanSwapchain(VulkanContext* pContext, VulkanSwapchain* pSwapchain);
	void VulkanSwapchainPresent(VulkanContext* pContext, VulkanSwapchain* pSwapchain);
	VulkanResultCode AcquireNextVulkanFramebuffer(VulkanContext* pContext, VulkanSwapchain* pSwapchain, VulkanFramebuffer** ppFramebuffer);
	void GetVulkanSwapchainFramebufferClass(VulkanSwapchain* pSwapchain, U64* fbClass);
	void GetVulkanSwapchainExtent(VulkanSwapchain* pSwapchain, VkExtent2D* pExtent);

	struct VulkanCommandBuffer
	{
		VkCommandBuffer commandBuffer;
		VkSemaphore finished;
	};

	VulkanResultCode CreateVulkanCommandBuffer(VulkanContext* pContext, VulkanCommandBuffer** ppCmdBuffer);
	void DestroyVulkanCommandBuffer(VulkanContext* pContext, VulkanCommandBuffer* pCmdBuffer);
	VulkanResultCode StartVulkanCommandRecording(VulkanCommandBuffer* pCmdBuffer);
	VulkanResultCode ResetVulkanCommandBuffer(VulkanCommandBuffer* pCmdBuffer);
	VulkanResultCode FinishVulkanCommandBuffer(VulkanCommandBuffer* pCmdBuffer);
	VulkanResultCode SubmitVulkanCommandBuffer(VulkanContext* pContext, VulkanCommandBuffer* pCmdBuffer);

	void CmdBeginVulkanRenderTarget(VulkanCommandBuffer* pCmdBuffer, VulkanFramebuffer* pFramebuffer);
	void CmdEndVulkanRenderTarget(VulkanCommandBuffer* pCmdBuffer);

	struct VulkanShaderCreateInfo
	{
		U32* pSPIR;
		U32 sizeInBytes;
	};

	struct VulkanShader
	{
		VkShaderModule shader;
	};

	VulkanResultCode CreateVulkanShader(VulkanContext* pContext, const VulkanShaderCreateInfo& info, VulkanShader** ppShader);
	void DestroyVulkanShader(VulkanContext* pContext, VulkanShader* pShader);

	struct VulkanVertexAttribute
	{
		U32 location;
		U32 componentLength;
		U32 offset;
	};

	struct VulkanVertexInputInfo
	{
		VulkanVertexAttribute* pAttribs;
		U32 attribCount;
		U32 stride;
	};

	enum VulkanPrimitiveTopology
	{
		VULKAN_TOPOLOGY_TRIANGLE_LIST
	};

	enum VulkanPolygonMode
	{
		VULKAN_POLYGON_MODE_FILL,
		VULKAN_POLYGON_MODE_LINE,
		VULKAN_POLYGON_MODE_POINT
	};

	enum VulkanCullMode
	{
		VULKAN_CULL_MODE_NONE = 0x0,
		VULKAN_CULL_MODE_FRONT = 0x1,
		VULKAN_CULL_MODE_BACK = 0x2,
		VULKAN_CULL_MODE_FRONT_AND_BACK = 0x3
	};

	enum VulkanFrontFace
	{
		VULKAN_FRONT_FACE_CCW,
		VULKAN_FRONT_FACE_CW
	};

	struct VulkanRasterizationInfo
	{
		VulkanPolygonMode polygonMode;
		VulkanCullMode cullMode;
		VulkanFrontFace frontFace;
		F32 lineWidth;
	};

	enum VulkanShaderPipelineStage
	{
		VULKAN_SHADER_STAGE_VERTEX,
		VULKAN_SHADER_STAGE_FRAGMENT
	};

	struct VulkanShaderStage
	{
		VulkanShader* pShader;
		VulkanShaderPipelineStage stage;
		const char* pEntrypoint;
	};

	struct VulkanPipelineCreateInfo
	{
		VulkanVertexInputInfo* pVertexInfos;
		U32 vertexInfoCount;
		VulkanPrimitiveTopology topology;
		VulkanRasterizationInfo* pRasterizationInfo;
		VulkanShaderStage* pStages;
		U32 stageCount;
		VkRenderPass renderPass;
	};

	struct VulkanPipeline
	{
		VkPipeline pipeline;
	};

	VulkanResultCode CreateVulkanPipeline(VulkanContext* pContext, const VulkanPipelineCreateInfo& info, VulkanPipeline** ppPipeline);
	void DestroyVulkanPipeline(VulkanContext* pContext, VulkanPipeline* pPipeline);

	void CmdBindVulkanPipeline(VulkanCommandBuffer* pCmdBuffer, VulkanPipeline* pPipeline);
	void CmdVulkanDrawVertices(VulkanCommandBuffer* pCmdBuffer, U32 offset, U32 size);
	void CmdSetVulkanViewport(VulkanCommandBuffer* pCmdBuffer, VkViewport viewport);
	void CmdSetVulkanScissor(VulkanCommandBuffer* pCmdBuffer, VkRect2D rect);
}