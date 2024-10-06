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

		float verts[] = {
			0.0f, 0.5f,
			0.5f, -0.5f,
			-0.5, -0.5f
		};

		GraphicsBufferCreateInfo bufferInfo = {};
		bufferInfo.dataSize = sizeof(verts);
		bufferInfo.pAlloc = &renderer.tempAllocator;
		bufferInfo.usage = BUFFER_USAGE_TRANSFER_SRC;
		GraphicsBuffer stagingBuffer = CreateGraphicsBuffer(&renderer.context, &bufferInfo);

		GraphicsBufferCreateInfo deviceBufferInfo = {};
		deviceBufferInfo.dataSize = sizeof(verts);
		deviceBufferInfo.pAlloc = &renderer.bufferAllocator;
		deviceBufferInfo.usage = BUFFER_USAGE_TRANSFER_DST | BUFFER_USAGE_VERTEX_BUFFER;
		renderer.vertBuffer = CreateGraphicsBuffer(&renderer.context, &deviceBufferInfo);
		
		void* pData = 0;
		MapDeviceMemory(&renderer.context, &renderer.tempAllocator, stagingBuffer.offset, sizeof(verts), &pData);
		memcpy(pData, verts, sizeof(verts));
		UnmapDeviceMemory(&renderer.context, &renderer.tempAllocator);

		BeginCommandBufferRecording(&renderer.commandBuffer);
		CmdCopyBuffer(&renderer.commandBuffer, &stagingBuffer, &renderer.vertBuffer, sizeof(verts), 0);
		EndCommandBufferRecording(&renderer.commandBuffer);
		SubmitCommandBuffer(&renderer.context, &renderer.commandBuffer);
		ResetCommandBuffer(&renderer.commandBuffer);

		DestroyGraphicsBuffer(&renderer.context, &renderer.tempAllocator, &stagingBuffer);

		return renderer;
	}

	void CreatePipeline(Renderer2D* pRenderer)
	{
		VertexBindingDesc mainBinding = {};
		mainBinding.binding = 0;
		mainBinding.stride = 4;

		VertexAttributeDesc attribDesc = {};
		attribDesc.shaderLocation = 0;
		attribDesc.byteOffset = 0;
		attribDesc.format = GRAPHICS_FORMATS_R32G32B32_SFLOAT;

		mainBinding.pAtrribs = &attribDesc;
		mainBinding.attribCount = 1;

		ShaderStageInfo shaders[2] = {
			{pRenderer->shaders[0], "main", SHADER_STAGES_VERTEX},
			{pRenderer->shaders[1], "main", SHADER_STAGES_FRAGMENT}
		};

		GraphicsPipelineCreateInfo pipelineInfo = {};
		pipelineInfo.renderPass.renderPass = pRenderer->swapchain.renderPass;
		pipelineInfo.pShaders = shaders;
		pipelineInfo.shaderCount = 2;
		pipelineInfo.pVertBindings = &mainBinding;
		pipelineInfo.bindingCount = 1;
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
		CmdBindVertexBuffer(&pRenderer->commandBuffer, &pRenderer->vertBuffer);

		CmdDraw(&pRenderer->commandBuffer, 0, 3);
		CmdEndFramebuffer(&pRenderer->commandBuffer);
		EndCommandBufferRecording(&pRenderer->commandBuffer);
		SubmitCommandBufferForSwapchain(&pRenderer->context, &pRenderer->commandBuffer, &pRenderer->swapchain);

		PresentSwapchainImage(&pRenderer->context, &pRenderer->swapchain);
	}

	void ShutdownRenderer2D(Renderer2D* pRenderer)
	{
		vkDeviceWaitIdle(pRenderer->context.device);

		DestroyGraphicsBuffer(&pRenderer->context, &pRenderer->bufferAllocator, &pRenderer->vertBuffer);

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