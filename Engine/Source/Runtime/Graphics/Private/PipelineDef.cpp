#include "PipelineDef.h"

namespace MarkTech
{
	GraphicsPipeline CreatePipelineWithPipelineDef(
		const GraphicsContext* pContext, 
		const Swapchain* pSwapchain,
		const PipelineDef* pPipelineDef)
	{
		GraphicsPipelineCreateInfo info = {};
		info.renderPass.renderPass = pSwapchain->renderPass;
		VertexBindingDesc binding;
		VertexAttributeDesc attribs[16];
		binding.binding = 0;
		binding.attribCount = pPipelineDef->vertAttribCount;
		binding.pAtrribs = attribs;

		U32 offset = 0;
		U32 location = 0;
		for (U32 i = 0; i < pPipelineDef->vertAttribCount; i++)
		{
			attribs[i].format = pPipelineDef->pVertAttribFmts[i];
			attribs[i].byteOffset = offset;
			attribs[i].shaderLocation = location;

			offset += GetGraphicsFormatSize(pPipelineDef->pVertAttribFmts[i]);
			if (GetGraphicsFormatSize(pPipelineDef->pVertAttribFmts[i]) > 4)
				location += 2;
			else
				location += 1;
		}

		info.pVertBindings = &binding;
		info.bindingCount = 1;

		ShaderStageInfo shaders[4];
		for (U32 i = 0; i < pPipelineDef->shaderCount; i++)
		{
			shaders[i].pEntrypoint = "main";
			shaders[i].stage = pPipelineDef->pShaders[i].stage;
		}

		return CreateGraphicsPipeline(pContext, &info);
	}
}