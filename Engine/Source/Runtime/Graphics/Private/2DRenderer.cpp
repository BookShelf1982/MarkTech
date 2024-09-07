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
		info.flags = GRAPHICS_CONTEXT_FLAGS_USE_WINDOW;
#ifdef DEBUG
		info.flags |= GRAPHICS_CONTEXT_FLAGS_DEBUG_MESSAGES;
#endif
		renderer.context = CreateGraphicsContext(&info);
		renderer.swapchain = CreateSwapchain(&renderer.context, pWindow);
		renderer.cmdPool = CreateCommandBufferPool(&renderer.context);
		renderer.commandBuffer = AllocateCommandBuffer(&renderer.context, &renderer.cmdPool);
		renderer.finishedRendering = CreateGraphicsSemaphore(&renderer.context);
		renderer.imageAvalible = CreateGraphicsSemaphore(&renderer.context);
		renderer.frameInFlight = CreateGraphicsFence(&renderer.context, true);
		return renderer;
	}

	void RenderFrame(Renderer2D* pRenderer)
	{
		WaitForFences(&pRenderer->context, &pRenderer->frameInFlight, 1);
		ResetFences(&pRenderer->context, &pRenderer->frameInFlight, 1);
		AquireNextSwapchainImage(&pRenderer->context, &pRenderer->swapchain, &pRenderer->imageAvalible, nullptr);

		ResetCommandBuffer(&pRenderer->commandBuffer);
		BeginCommandBufferRecording(&pRenderer->commandBuffer);
		CmdBindSwapchainFramebuffer(&pRenderer->commandBuffer, &pRenderer->swapchain);
		CmdEndFramebuffer(&pRenderer->commandBuffer);
		EndCommandBufferRecording(&pRenderer->commandBuffer);
		SubmitCommandBuffer(&pRenderer->context, &pRenderer->commandBuffer, nullptr, 0, &pRenderer->finishedRendering, 1, &pRenderer->frameInFlight);

		PresentSwapchainImage(&pRenderer->context, &pRenderer->swapchain, &pRenderer->finishedRendering, 1);
	}

	void ShutdownRenderer2D(Renderer2D* pRenderer)
	{
		FreeCommandBuffer(&pRenderer->context, &pRenderer->commandBuffer, &pRenderer->cmdPool);
		DestroyCommandBufferPool(&pRenderer->context, &pRenderer->cmdPool);
		DestroySwapchain(&pRenderer->context, &pRenderer->swapchain);
		DestroyGraphicsContext(&pRenderer->context);
	}
}