#pragma once
#include "GraphicsInterface.h"
#include "Window.h"

namespace MarkTech
{
#define MT_MAX_SHADERS 16

	struct Renderer2D
	{
		GraphicsContext context;
		Swapchain swapchain;
		CommandBufferPool cmdPool;
		CommandBuffer commandBuffer;
		GraphicsPipeline pipeline;
		ShaderModule shaders[MT_MAX_SHADERS];
		U32 loadedShaders;
		DeviceAllocator bufferAllocator;
		DeviceAllocator tempAllocator;
	};

	void LoadShader(Renderer2D* pRenderer, U32* pCode, U64 codeSizeInBytes);
	Renderer2D InitRenderer2D(const Window* pWindow);
	void CreatePipeline(Renderer2D* pRenderer);
	void RenderFrame(Renderer2D* pRenderer);
	void ShutdownRenderer2D(Renderer2D* pRenderer);
}