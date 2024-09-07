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

	void CreatePipeline(Renderer2D* pRenderer)
	{
		ShaderStageInfo shaders[2] = {
			{pRenderer->shaders[0], "main", SHADER_STAGES_VERTEX},
			{pRenderer->shaders[1], "main", SHADER_STAGES_FRAGMENT}
		};

		GraphicsPipelineCreateInfo pipelineInfo = {};
		pipelineInfo.renderPass.renderpass = pRenderer->swapchain.renderPass;
		pipelineInfo.pShaders = shaders;
		pipelineInfo.shaderCount = 2;
		pRenderer->pipeline = CreateGraphicsPipeline(&pRenderer->context, &pipelineInfo);
	}

	void LoadShader(Renderer2D* pRenderer, U32* pCode, U64 codeSizeInBytes)
	{
		ShaderCreateInfo info = {};
		info.pCode = pCode;
		info.sizeInBytes = codeSizeInBytes;
		pRenderer->shaders[pRenderer->loadedShaders] = CreateShaderModule(&pRenderer->context, &info);
		pRenderer->loadedShaders++;
	}

	void RenderFrame(Renderer2D* pRenderer)
	{
		WaitForFences(&pRenderer->context, &pRenderer->frameInFlight, 1);
		ResetFences(&pRenderer->context, &pRenderer->frameInFlight, 1);
		AquireNextSwapchainImage(&pRenderer->context, &pRenderer->swapchain, &pRenderer->imageAvalible, nullptr);

		ResetCommandBuffer(&pRenderer->commandBuffer);
		BeginCommandBufferRecording(&pRenderer->commandBuffer);
		CmdBindSwapchainFramebuffer(&pRenderer->commandBuffer, &pRenderer->swapchain);
		CmdBindPipeline(&pRenderer->commandBuffer, &pRenderer->pipeline);

		ViewportScissor viewport = {};
		viewport.offsetX = 0;
		viewport.offsetY = 0;
		viewport.width = (F32)pRenderer->swapchain.framebufferExtent.width;
		viewport.height = (F32)pRenderer->swapchain.framebufferExtent.height;
		viewport.extentX = pRenderer->swapchain.framebufferExtent.width;
		viewport.extentY = pRenderer->swapchain.framebufferExtent.height;
		CmdSetViewportScissor(&pRenderer->commandBuffer, &viewport);

		CmdDraw(&pRenderer->commandBuffer, 0, 3);
		CmdEndFramebuffer(&pRenderer->commandBuffer);
		EndCommandBufferRecording(&pRenderer->commandBuffer);
		SubmitCommandBuffer(&pRenderer->context, &pRenderer->commandBuffer, &pRenderer->finishedRendering, 1, &pRenderer->imageAvalible, 1, &pRenderer->frameInFlight);

		PresentSwapchainImage(&pRenderer->context, &pRenderer->swapchain, &pRenderer->finishedRendering, 1);
	}

	void ShutdownRenderer2D(Renderer2D* pRenderer)
	{
		vkDeviceWaitIdle(pRenderer->context.device);

		for (U32 i = 0; i < pRenderer->loadedShaders; i++)
			DestroyShaderModule(&pRenderer->context, &pRenderer->shaders[i]);

		DestroyGraphicsSemaphore(&pRenderer->context, &pRenderer->finishedRendering);
		DestroyGraphicsSemaphore(&pRenderer->context, &pRenderer->imageAvalible);
		DestroyGraphicsFence(&pRenderer->context, &pRenderer->frameInFlight);
		DestroyGraphicsPipeline(&pRenderer->context, &pRenderer->pipeline);
		FreeCommandBuffer(&pRenderer->context, &pRenderer->commandBuffer, &pRenderer->cmdPool);
		DestroyCommandBufferPool(&pRenderer->context, &pRenderer->cmdPool);
		DestroySwapchain(&pRenderer->context, &pRenderer->swapchain);
		DestroyGraphicsContext(&pRenderer->context);
	}
}