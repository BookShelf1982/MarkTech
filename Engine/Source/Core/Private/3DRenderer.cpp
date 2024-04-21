#include "3DRenderer.h"
#include <fstream>

comp_shader_t C3DRenderer::ReadShaderFromFile(const char* pFilepath)
{
	comp_shader_t result = {};

	std::fstream file(pFilepath, std::ios::in | std::ios::binary);
	if (!file.is_open())
		return result;

	file.seekg(0, std::ios::end);
	size_t fileSize = file.tellg();
	file.seekg(0);
	uint32_t* pData = new uint32_t[fileSize];
	file.read((char*)pData, fileSize);
	file.close();

	result.nDataSize = (uint32_t)fileSize;
	result.pData = pData;

	return result;
}

C3DRenderer::C3DRenderer(HWND hwnd)
{
	m_pGraphicsModule = IModuleLoader::LoadModule("Graphics.dll");
	
	// -- Graphics module -- //
	m_pfnGraphicsFuncs.pfnCreateGraphicsContext = (PFN_CreateGraphicsContext)m_pGraphicsModule->GetModuleProcAddress("CreateGraphicsContext");
	m_pfnGraphicsFuncs.pfnDestroyGraphicsContext = (PFN_DestroyGraphicContext)m_pGraphicsModule->GetModuleProcAddress("DestroyGraphicsContext");

	m_pGraphicsContext = m_pfnGraphicsFuncs.pfnCreateGraphicsContext(EGraphicsAPI::OpenGL, hwnd);

	// vert buffer
	float positions[6] = {
		0.0f, 0.5f,
		0.5f, -0.5f,
		-0.5f, -0.5f
	};

	MCreateBufferInfo bufferInfo = {};
	bufferInfo.pData = positions;
	bufferInfo.nSize = sizeof(positions);
	bufferInfo.target = EBindingTarget::VERTEX_ARRAY;
	bufferInfo.usage = EBufferUsage::STATIC;

	m_Buffer = UploadToGPU(bufferInfo);

	// shaders
	comp_shader_t vert = ReadShaderFromFile("vert.spv");
	comp_shader_t frag = ReadShaderFromFile("frag.spv");

	MCreateShaderInfo fragInfo = {};
	fragInfo.pEntrypoint = "main";
	fragInfo.nShaderDataSize = frag.nDataSize;
	fragInfo.pShaderData = frag.pData;
	fragInfo.stage = EShaderStage::FRAGMENT;

	m_FragShader = CreateShader(fragInfo);

	MCreateShaderInfo vertInfo = {};
	vertInfo.pEntrypoint = "main";
	vertInfo.nShaderDataSize = vert.nDataSize;
	vertInfo.pShaderData = vert.pData;
	vertInfo.stage = EShaderStage::VERTEX;

	m_VertShader = CreateShader(vertInfo);

	MShaderProgramElement vertElement = {};
	vertElement.m_pShader = m_VertShader.GetAddress();

	MShaderProgramElement fragElement = {};
	fragElement.m_pShader = m_FragShader.GetAddress();

	CTList<MShaderProgramElement> list(2);
	list.Push(vertElement);
	list.Push(fragElement);

	MShaderProgramInfo programInfo = {};
	programInfo.nNumPrograms = list.Size();
	programInfo.pPrograms = list.Data();

	MInputLayoutElement posElement = {};
	posElement.nIndex = 0;
	posElement.nOffsetInBytes = 0;
	posElement.type = EElementType::FLOAT2;

	CTList<MInputLayoutElement> inputElements(1);
	inputElements.Push(posElement);

	// input layout
	MInputLayoutInfo inputLayoutInfo = {};
	inputLayoutInfo.pElements = inputElements.Data();
	inputLayoutInfo.nElementCount = inputElements.Size();
	inputLayoutInfo.nStride = sizeof(float) * 2;

	// Pipeline
	MCreatePipelineInfo pipelineInfo = {};
	pipelineInfo.shaderProgrm = programInfo;
	pipelineInfo.inputLayout = inputLayoutInfo;

	m_Pipeline = m_pGraphicsContext->CreatePipeline(pipelineInfo);

	frag.Release();
	vert.Release();
}

C3DRenderer::~C3DRenderer()
{
	m_pfnGraphicsFuncs.pfnDestroyGraphicsContext(m_pGraphicsContext);
	delete m_pGraphicsModule;
}

void C3DRenderer::RenderFrame()
{
	m_pGraphicsContext->Test();
	m_pGraphicsContext->BindBuffer(m_Buffer.GetAddress(), EBindingTarget::VERTEX_ARRAY);
	m_pGraphicsContext->BindPipeline(m_Pipeline.GetAddress());
	m_pGraphicsContext->DrawVertices(EDrawMode::TRIANGLES, 0, 2);
	m_pGraphicsContext->SwapImages();
}

IBuffer* C3DRenderer::UploadToGPU(MCreateBufferInfo info)
{
	return m_pGraphicsContext->CreateBuffer(info);
}

IShader* C3DRenderer::CreateShader(MCreateShaderInfo info)
{
	return m_pGraphicsContext->CreateShader(info);
}
