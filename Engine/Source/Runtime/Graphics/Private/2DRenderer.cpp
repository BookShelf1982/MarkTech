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

		renderer.bufferAllocator = AllocateDeviceAllocator(&renderer.context, MEGABYTE, ALLOCATION_TYPE_DEVICE);
		renderer.tempAllocator = AllocateDeviceAllocator(&renderer.context, 64 * KILOBYTE, ALLOCATION_TYPE_APPLICATION);
		
		GraphicsBufferCreateInfo bufferInfo = {};
		bufferInfo.dataSize = 1024;
		bufferInfo.pAlloc = &renderer.tempAllocator;
		bufferInfo.pData = nullptr;
		bufferInfo.usage = BUFFER_USAGE_VERTEX_BUFFER;
		GraphicsBuffer buffer = CreateGraphicsBuffer(&renderer.context, &bufferInfo);
		DestroyGraphicsBuffer(&renderer.context, &buffer);

		return renderer;
	}

	void CreatePipeline(Renderer2D* pRenderer)
	{
		ShaderStageInfo shaders[2] = {
			{pRenderer->shaders[0], "main", SHADER_STAGES_VERTEX},
			{pRenderer->shaders[1], "main", SHADER_STAGES_FRAGMENT}
		};

		GraphicsPipelineCreateInfo pipelineInfo = {};
		pipelineInfo.renderPass.renderPass = pRenderer->swapchain.renderPass;
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
		AquireNextSwapchainImage(&pRenderer->context, &pRenderer->swapchain);

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
		SubmitCommandBufferForSwapchain(&pRenderer->context, &pRenderer->commandBuffer, &pRenderer->swapchain);

		PresentSwapchainImage(&pRenderer->context, &pRenderer->swapchain);
	}

	void ShutdownRenderer2D(Renderer2D* pRenderer)
	{
		vkDeviceWaitIdle(pRenderer->context.device);
		FreeDeviceAllocator(&pRenderer->context, &pRenderer->tempAllocator);
		FreeDeviceAllocator(&pRenderer->context, &pRenderer->bufferAllocator);

		for (U32 i = 0; i < pRenderer->loadedShaders; i++)
			DestroyShaderModule(&pRenderer->context, &pRenderer->shaders[i]);

		DestroyGraphicsPipeline(&pRenderer->context, &pRenderer->pipeline);
		FreeCommandBuffer(&pRenderer->context, &pRenderer->commandBuffer, &pRenderer->cmdPool);
		DestroyCommandBufferPool(&pRenderer->context, &pRenderer->cmdPool);
		DestroySwapchain(&pRenderer->context, &pRenderer->swapchain);
		DestroyGraphicsContext(&pRenderer->context);
	}
}