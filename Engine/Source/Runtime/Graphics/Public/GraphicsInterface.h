#include <PrimitiveTypes.h>

#ifdef MT_PLATFORM_WINDOWS
#include <Windows.h>
#endif
#include <Window.h>

namespace MarkTech
{
	enum GraphicsAPI
	{
		GRAPHICS_API_NONE,
		GRAPHICS_API_OPENGL,
		GRAPHICS_API_VULKAN
	};

	enum ResultCode
	{
		RC_SUCCESS,
		RC_FAILED
	};

	enum ContextCreateFlagBits
	{
		CONTEXT_FLAGS_DEBUG_MESSAGES = 0x1
	};

	typedef U32 ContextCreateFlags;

	typedef void* GraphicsContext;
	typedef void* Swapchain;
	typedef void* CommandBuffer;
	typedef void* Shader;
	typedef void* GraphicsPipeline;
	typedef void* Framebuffer;
	typedef void* FramebufferClass;
	
	struct ApplicationInfo
	{
		const char* pName;
		U32 verMajor;
		U32 verMinor;
		U32 verPatch;
	};

	struct GraphicsContextCreateInfo
	{
		ApplicationInfo* pAppInfo;
		ContextCreateFlags flags;
		GraphicsAPI api;
	};

	ResultCode CreateGraphicsContext(const GraphicsContextCreateInfo& info, GraphicsContext* pContext);
	void DestroyGraphicsContext(GraphicsContext context);
	void WaitDeviceIdle(GraphicsContext context);

	struct Viewport
	{
		F32 x, y;
		F32 width, height;
		F32 minDepth, maxDepth;
	};

	struct Extent2D
	{
		U32 width, height;
	};

	struct Rect2D
	{
		I32 x, y;
		U32 width, height;
	};

	enum PresentationMode
	{
		PRESENTATION_MODE_FIFO,
		PRESENTATION_MODE_IMMEDIATE
	};

	struct SwapchainCreateInfo
	{
		Window* pWindow;
		PresentationMode presentationMode;
		Swapchain oldSwapchain;
	};

	ResultCode CreateSwapchain(GraphicsContext context, const SwapchainCreateInfo& info, Swapchain* pSwapchain);
	void DestroySwapchain(GraphicsContext context, Swapchain swapchain);
	ResultCode AcquireNextSwapchainImage(GraphicsContext context, Swapchain swapchain, Framebuffer* pFramebuffer);
	void SwapchainPresent(GraphicsContext context, Swapchain swapchain);
	void GetSwapchainFramebufferClass(Swapchain swapchain, FramebufferClass fbClass);
	void GetSwapchainExtent(Swapchain swapchain, Extent2D* pExtent);

	ResultCode CreateCommandBuffer(GraphicsContext context, CommandBuffer* pCmdBuffer);
	void DestroyCommandBuffer(GraphicsContext context, CommandBuffer cmdBuffer);
	ResultCode StartCommandRecording(CommandBuffer cmdBuffer);
	ResultCode ResetCommandBuffer(CommandBuffer cmdBuffer);
	ResultCode FinishCommandBuffer(CommandBuffer cmdBuffer);
	ResultCode SubmitCommandBuffer(GraphicsContext context, CommandBuffer cmdBuffer);

	struct FramebufferCreateInfo
	{

	};

	ResultCode CreateFrameBuffer(GraphicsContext context, Framebuffer* pFB);
	void DestroyFramebuffer(GraphicsContext context, Framebuffer* pFB);
	void CmdBeginRenderTarget(CommandBuffer cmdBuffer, Framebuffer framebuffer);
	void CmdEndRenderTarget(CommandBuffer cmdBuffer);

	struct ShaderCreateInfo
	{
		U32* pSPIR;
		U32 sizeInBytes;
	};

	ResultCode CreateShader(GraphicsContext context, const ShaderCreateInfo& info, Shader* pShader);
	void DestroyShader(GraphicsContext context, Shader shader);

	struct VertexAttribute
	{
		U32 location;
		U32 componentLength;
		U32 offset;
	};

	struct VertexInputInfo
	{
		VertexAttribute* pAttribs;
		U32 attribCount;
		U32 stride;
	};

	enum PrimitiveTopology
	{
		TOPOLOGY_TRIANGLE_LIST
	};

	enum PolygonMode
	{
		POLYGON_MODE_FILL,
		POLYGON_MODE_LINE,
		POLYGON_MODE_POINT
	};

	enum CullMode
	{
		CULL_MODE_NONE = 0x0,
		CULL_MODE_FRONT = 0x1,
		CULL_MODE_BACK = 0x2,
		CULL_MODE_FRONT_AND_BACK = 0x3
	};

	enum FrontFace
	{
		FRONT_FACE_CCW,
		FRONT_FACE_CW
	};

	struct RasterizationInfo
	{
		PolygonMode polygonMode;
		CullMode cullMode;
		FrontFace frontFace;
		F32 lineWidth;
	};

	enum ShaderPipelineStage
	{
		SHADER_PIPELINE_STAGE_VERTEX,
		SHADER_PIPELINE_STAGE_FRAGMENT
	};

	struct ShaderStage
	{
		Shader shader;
		ShaderPipelineStage stage;
		const char* pEntrypoint;
	};

	struct GraphicsPipelineCreateInfo
	{
		VertexInputInfo* pVertexInfos;
		U32 vertexInfoCount;
		PrimitiveTopology topology;
		RasterizationInfo* pRasterizationInfo;
		ShaderStage* pStages;
		U32 stageCount;
		FramebufferClass framebufferClass;
	};

	ResultCode CreateGraphicsPipeline(GraphicsContext context, const GraphicsPipelineCreateInfo& info, GraphicsPipeline* pPipeline);
	void DestroyGraphicsPipeline(GraphicsContext context, GraphicsPipeline pipeline);
	void CmdBindPipeline(CommandBuffer cmdBuffer, GraphicsPipeline pipeline);
	void CmdDrawVertices(CommandBuffer cmdBuffer, U32 offset, U32 size);
	void CmdSetViewport(CommandBuffer cmdBuffer, Viewport viewport);
	void CmdSetScissor(CommandBuffer cmdBuffer, Rect2D rect);
}