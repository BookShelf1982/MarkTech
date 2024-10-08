#pragma once
#include <PrimitiveTypes.h>
#ifdef MT_PLATFORM_WINDOWS
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#include "Window.h"
#include <volk.h>
#include <PoolAllocator.h>

namespace MarkTech
{
#define MT_MAX_SWAPCHAIN_IMAGES 2
#define MT_MAX_SEMAPHORES 16
#define MT_MAX_FENCES 16
#define MT_MAX_PIPELINE_SHADERS 8

	enum GraphicsFormats
	{
		GRAPHICS_FORMATS_R32_SFLOAT,
		GRAPHICS_FORMATS_R32G32_SFLOAT,
		GRAPHICS_FORMATS_R32G32B32_SFLOAT,
		GRAPHICS_FORMATS_R32G32B32A32_SFLOAT,
		GRAPHICS_FORMATS_R32_UINT,
	};

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

	struct RenderPass
	{
		VkRenderPass renderPass;
	};

	struct Swapchain
	{
		U32 framebufferIndex;
		VkExtent2D framebufferExtent;
		VkSurfaceKHR windowSurface;
		VkSwapchainKHR swapchain;
		U32 swapchainImageCount;
		VkImage swapchainImages[MT_MAX_SWAPCHAIN_IMAGES];
		VkImageView swapchainImageViews[MT_MAX_SWAPCHAIN_IMAGES];
		VkFramebuffer framebuffers[MT_MAX_SWAPCHAIN_IMAGES];
		VkRenderPass renderPass;
		VkFence frameInFlight;
		VkSemaphore imageAvalible;
		VkSemaphore finishedRendering;
	};

	enum ShaderStage : U8
	{
		SHADER_STAGES_VERTEX = 0x02,
		SHADER_STAGES_FRAGMENT = 0x04
	};

	struct ShaderCreateInfo
	{
		U32* pCode;
		U64 sizeInBytes;
	};

	struct ShaderModule
	{
		VkShaderModule shader;
	};

	struct ShaderStageInfo
	{
		ShaderModule shader;
		const char* pEntrypoint;
		ShaderStage stage;
	};

	struct VertexAttributeDesc
	{
		U32 shaderLocation;
		GraphicsFormats format;
		U32 byteOffset;
	};

	struct VertexBindingDesc
	{
		U32 binding;
		U32 stride;
		VertexAttributeDesc* pAtrribs;
		U32 attribCount;
	};

	struct GraphicsPipelineCreateInfo
	{
		VertexBindingDesc* pVertBindings;
		U32 bindingCount;
		ShaderStageInfo* pShaders;
		U32 shaderCount;
		RenderPass renderPass;
	};

	struct GraphicsPipeline
	{
		VkPipeline pipeline;
		VkPipelineLayout pipelineLayout;
	};

	struct CommandBufferPool
	{
		VkCommandPool commandPool;
	};

	struct CommandBuffer
	{
		VkCommandBuffer commandBuffer;
	};

	struct FreeMemory
	{
		VkDeviceAddress offset;
		VkDeviceSize size;
		FreeMemory* pNext;
	};

	struct DeviceAllocator
	{
		VkDeviceMemory deviceAllocation;
		FreeMemory* pFreeBlocks;
		PoolAllocator pool;
	};

	enum AllocationType
	{
		ALLOCATION_TYPE_DEVICE,
		ALLOCATION_TYPE_APPLICATION
	};

	enum BufferUsage
	{
		BUFFER_USAGE_VERTEX_BUFFER = 0x1,
		BUFFER_USAGE_INDEX_BUFFER = 0x2,
		BUFFER_USAGE_UNIFORM_BUFFER = 0x4,
		BUFFER_USAGE_TRANSFER_SRC = 0x8,
		BUFFER_USAGE_TRANSFER_DST = 0x10
	};

	struct GraphicsBufferCreateInfo
	{
		U64 dataSize;
		U32 usage;
		DeviceAllocator* pAlloc;
	};

	struct GraphicsBuffer
	{
		VkBuffer buffer;
		VkDeviceAddress offset;
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

	/*struct GraphicsFence
	{
		VkFence fence;
	};

	struct GraphicsSemaphore
	{
		VkSemaphore semaphore;
	};*/

	struct ViewportScissor
	{
		F32 x, y, width, height;
		U32 extentX, extentY;
		I32 offsetX, offsetY;
	};

	GraphicsContext CreateGraphicsContext(const GraphicsContextCreateInfo* pInfo);
	void DestroyGraphicsContext(GraphicsContext* pContext);

	/*GraphicsSemaphore CreateGraphicsSemaphore(const GraphicsContext* pContext);
	void DestroyGraphicsSemaphore(const GraphicsContext* pContext, GraphicsSemaphore* pSemaphore);

	GraphicsFence CreateGraphicsFence(const GraphicsContext* pContext, bool signaled);
	void DestroyGraphicsFence(const GraphicsContext* pContext, GraphicsFence* pFence);
	void WaitForFences(const GraphicsContext* pContext, GraphicsFence* pFences, U32 fenceCount);
	void ResetFences(const GraphicsContext* pContext, GraphicsFence* pFences, U32 fenceCount);*/

	Swapchain CreateSwapchain(const GraphicsContext* pContext, const Window* pWindow);
	void DestroySwapchain(const GraphicsContext* pContext, Swapchain* pSwapchain);
	void AquireNextSwapchainImage(const GraphicsContext* pContext, Swapchain* pSwapchain);
	void PresentSwapchainImage(const GraphicsContext* pContext, Swapchain* pSwapchain);

	ShaderModule CreateShaderModule(const GraphicsContext* pContext, const ShaderCreateInfo* pInfo);
	void DestroyShaderModule(const GraphicsContext* pContext, ShaderModule* pShader);

	GraphicsPipeline CreateGraphicsPipeline(const GraphicsContext* pContext, const GraphicsPipelineCreateInfo* pInfo);
	void DestroyGraphicsPipeline(const GraphicsContext* pContext, GraphicsPipeline* pPipeline);

	DeviceAllocator AllocateDeviceAllocator(const GraphicsContext* pContext, U64 allocSize, AllocationType type);
	void FreeDeviceAllocator(const GraphicsContext* pContext, DeviceAllocator* pAlloc);

	GraphicsBuffer CreateGraphicsBuffer(const GraphicsContext* pContext, const GraphicsBufferCreateInfo* pInfo);
	void DestroyGraphicsBuffer(const GraphicsContext* pContext, DeviceAllocator* pAlloc, GraphicsBuffer* pBuffer);
	void MapDeviceMemory(const GraphicsContext* pContext, DeviceAllocator* pAlloc, U64 offset, U64 size, void** ppPtr);
	void UnmapDeviceMemory(const GraphicsContext* pContext, DeviceAllocator* pAlloc);

	CommandBufferPool CreateCommandBufferPool(const GraphicsContext* pContext);
	void DestroyCommandBufferPool(const GraphicsContext* pContext, CommandBufferPool* pPool);
	void ResetCommandBufferPool(const GraphicsContext* pContext, CommandBufferPool* pPool);

	CommandBuffer AllocateCommandBuffer(const GraphicsContext* pContext, const CommandBufferPool* pPool);
	void ResetCommandBuffer(CommandBuffer* pCmdBuffer);
	void FreeCommandBuffer(const GraphicsContext* pContext, CommandBuffer* pCmdBuffer, const CommandBufferPool* pPool);
	void BeginCommandBufferRecording(CommandBuffer* pCmdBuffer);
	void EndCommandBufferRecording(CommandBuffer* pCmdBuffer);

	void SubmitCommandBufferForSwapchain(const GraphicsContext* pContext, CommandBuffer* pCmdBuffer, Swapchain* pSwapchain);
	void SubmitCommandBuffer(const GraphicsContext* pContext, CommandBuffer* pCmdBuffer);

	void CmdBindSwapchainFramebuffer(CommandBuffer* pCmdBuffer, const Swapchain* pSwapchain);
	void CmdEndFramebuffer(CommandBuffer* pCmdBuffer);
	void CmdBindPipeline(CommandBuffer* pCmdBuffer, const GraphicsPipeline* pPipeline);
	void CmdDraw(CommandBuffer* pCmdBuffer, U32 offset, U32 length);
	void CmdSetViewportScissor(CommandBuffer* pCmdBuffer, const ViewportScissor* pViewportScissor);
	void CmdCopyBuffer(CommandBuffer* pCmdBuffer, GraphicsBuffer* src, GraphicsBuffer* dst, U64 size, U64 offset);
	void CmdBindVertexBuffer(CommandBuffer* pCmdBuffer, GraphicsBuffer* pBuffer);
}