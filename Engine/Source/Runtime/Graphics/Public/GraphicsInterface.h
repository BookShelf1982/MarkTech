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
}