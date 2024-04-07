#include "ResourceCompiler.h"

CResourceCompiler::CResourceCompiler()
{
}

CResourceCompiler::~CResourceCompiler()
{
}

MCompiledShader CResourceCompiler::CompileShader(const char* pSource, uint32_t nType, const char* pFilename)
{
	MCompiledShader shader = {};
	shaderc::Compiler compiler;
	shaderc::CompileOptions compilerOptions;
	shaderc_shader_kind shaderType;

	switch (nType)
	{
	default:
		shaderType = shaderc_vertex_shader;
		break;
	case 0:
		shaderType = shaderc_vertex_shader;
		break;
	case 1:
		shaderType = shaderc_fragment_shader;
		break;
	}

	shaderc::CompilationResult compResult = compiler.CompileGlslToSpv(pSource, shaderType, pFilename, compilerOptions);

	if (compResult.GetCompilationStatus() != shaderc_compilation_status_success)
	{
		printf("Failed to compile shader: %s", compResult.GetErrorMessage().c_str());
		return shader;
	}

	uint32_t nDataSize = compResult.end() - compResult.begin();

	uint32_t* pBytecode = new uint32_t[nDataSize];

	memcpy(pBytecode, compResult.begin(), nDataSize);

	shader.pData = pBytecode;
	shader.nDataSize = nDataSize;

	return shader;
}

void CResourceCompiler::ReleaseCompiledShader(MCompiledShader& shader)
{
	delete[] shader.pData;
}
