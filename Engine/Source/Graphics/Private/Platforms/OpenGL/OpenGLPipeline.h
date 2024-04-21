#pragma once
#include "Pipeline.h"

class COpenGLPipeline : public IPipeline
{
public:
	COpenGLPipeline(MCreatePipelineInfo info);
	~COpenGLPipeline();

	uint32_t GetShaderProgramId() { return m_nShaderProgramId; }
	MRasterizerInfo GetRasterInfo() { return m_RasterInfo; }
	MInputLayoutInfo GetInputLayout() { return m_InputLayout; }
private:
	uint32_t m_nShaderProgramId;
	MRasterizerInfo m_RasterInfo;
	MInputLayoutInfo m_InputLayout;

	uint32_t CreateShaderProgram(MShaderProgramInfo info);
};