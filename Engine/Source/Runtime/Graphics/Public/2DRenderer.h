#pragma once
#include "GraphicsInterface.h"
#include "Window.h"

namespace MarkTech
{
	struct Renderer2D
	{
		GraphicsContext context;
		Swapchain swapchain;
		CommandBufferPool cmdPool;
		CommandBuffer commandBuffer;
		GraphicsFence frameInFlight;
		GraphicsSemaphore imageAvalible;
		GraphicsSemaphore finishedRendering;
	};

	Renderer2D InitRenderer2D(const Window* pWindow);
	void RenderFrame(Renderer2D* pRenderer);
	void ShutdownRenderer2D(Renderer2D* pRenderer);
}