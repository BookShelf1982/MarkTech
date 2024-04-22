#include "OpenGLPipeline.h"
#include "OpenGLShader.h"

COpenGLPipeline::COpenGLPipeline(MCreatePipelineInfo info)
	:m_nShaderProgramId(0), m_RasterInfo(info.rasterizerInfo), m_InputLayout(info.inputLayout)
{
	m_nShaderProgramId = CreateShaderProgram(info.shaderProgrm);
	m_InputLayout.pElements = new MInputLayoutElement[info.inputLayout.nElementCount];
	memcpy(m_InputLayout.pElements, info.inputLayout.pElements, info.inputLayout.nElementCount * sizeof(MInputLayoutElement));
}

COpenGLPipeline::~COpenGLPipeline()
{
	glDeleteProgram(m_nShaderProgramId);
	delete[] m_InputLayout.pElements;
}

uint32_t COpenGLPipeline::CreateShaderProgram(MShaderProgramInfo info)
{
	uint32_t nProgram = glCreateProgram();
	for (uint32_t i = 0; i < info.nNumPrograms; i++)
	{
		glAttachShader(nProgram, dynamic_cast<COpenGLShader*>(info.pPrograms[i].m_pShader)->GetId());
	}

	glLinkProgram(nProgram);

	GLint isLinked = 0;
	glGetProgramiv(nProgram, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE)
	{
		// We don't need the program anymore.
		glDeleteProgram(nProgram);
		MASSERT(false);
		return 0;
	}

	/*for (uint32_t i = 0; i < info.nNumPrograms; i++)
	{
		glDetachShader(nProgram, dynamic_cast<COpenGLShader*>(info.pPrograms[i].m_pShader)->GetId());
	}*/

	return nProgram;
}
