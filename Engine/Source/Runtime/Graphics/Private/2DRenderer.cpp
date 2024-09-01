#include "2DRenderer.h"

namespace MarkTech
{
	Renderer2D InitRenderer2D(const Window* pWindow)
	{
		Renderer2D renderer = {};

		GraphicsContextCreateInfo info = {};
		info.appInfo.pAppName = "SandboxApp";
		info.appInfo.majorVersion = 1;
		info.appInfo.minorVersion = 0;
		info.appInfo.patchVersion = 0;
#ifdef DEBUG
		info.flags = GRAPHICS_CONTEXT_FLAGS_DEBUG_MESSAGES;
#endif
		renderer.context = CreateGraphicsContext(&info);
		return renderer;
	}

	void ShutdownRenderer2D(Renderer2D* pRenderer)
	{
		DestroyGraphicsContext(&pRenderer->context);
	}
}