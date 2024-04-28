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
	char* pData = new char[fileSize];
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
	Vert verts[3] = {
		{0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f},
		{0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f},
		{-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f}
	};

	MCreateBufferInfo bufferInfo = {};
	bufferInfo.pData = verts;
	bufferInfo.nSize = sizeof(verts);
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
	posElement.type = EElementType::FLOAT3;

	MInputLayoutElement colElement = {};
	colElement.nIndex = 1;
	colElement.nOffsetInBytes = sizeof(float) * 3;
	colElement.type = EElementType::FLOAT3;

	CTList<MInputLayoutElement> inputElements(2);
	inputElements.Push(posElement);
	inputElements.Push(colElement);

	// input layout
	MInputLayoutInfo inputLayoutInfo = {};
	inputLayoutInfo.pElements = inputElements.Data();
	inputLayoutInfo.nElementCount = inputElements.Size();
	inputLayoutInfo.nStride = sizeof(Vert);

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
	delete m_pGraphicsContext;
	delete m_pGraphicsModule;
}

void C3DRenderer::RenderFrame()
{
	m_pGraphicsContext->Test();
	m_pGraphicsContext->BindBuffer(m_Buffer.GetAddress(), EBindingTarget::VERTEX_ARRAY);
	m_pGraphicsContext->BindPipeline(m_Pipeline.GetAddress());
	m_pGraphicsContext->DrawVertices(EDrawMode::TRIANGLES, 0, 3);

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
