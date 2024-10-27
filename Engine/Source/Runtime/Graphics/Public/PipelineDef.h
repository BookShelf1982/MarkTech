#pragma once
#include <2DRenderer.h>

namespace MarkTech
{
	struct ShaderDef
	{
		U32 id;
		ShaderStage stage;
	};

	struct PipelineDef
	{
		GraphicsFormats* pVertAttribFmts;
		U32 vertAttribCount;
		ShaderDef* pShaders;
		U32 shaderCount;
	};

	GraphicsPipeline CreatePipelineWithPipelineDef(const GraphicsContext* pContext,
		const Swapchain* pSwapchain,
		const PipelineDef* pPipelineDef);
}