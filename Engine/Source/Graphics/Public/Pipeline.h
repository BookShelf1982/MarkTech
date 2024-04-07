#pragma once
#include "Core.h"
#include "Shader.h"

// -- Input Layout -- //

enum class EElementType
{
	FLOAT,
	FLOAT2,
	FLOAT3,
	FLOAT4,
};

struct MInputLayoutElement
{
	uint32_t nIndex;
	EElementType type;
	uint32_t nOffsetInBytes;
};

struct MInputLayoutInfo
{
	MInputLayoutElement* pElements;
	uint32_t nElementCount;
};

// -- Rasterizer -- //

enum class EPolygonMode
{
	FILL,
	WIREFRAME
};

enum class EFrontFace
{
	CLOCKWISE,
	COUNTER_CLOCKWISE
};

enum class ECullMode
{
	FRONT,
	BACK,
	FRONT_AND_BACK
};

struct MRasterizerInfo
{
	EPolygonMode polygonMode;
	EFrontFace frontFace;
	ECullMode cullMode;
};

// -- Shaders -- //

struct MShaderProgramInfo
{
	EShaderStage stage;
	const char* pEntrypoint;
};

// -- Pipeline -- //

struct MCreatePipelineInfo
{
	MInputLayoutInfo vertexInputLayout;
	MRasterizerInfo rasterizerInfo;
	MShaderProgramInfo vertShader;
	MShaderProgramInfo fragShader;
};

class IPipeline
{
public:
	IPipeline() {}
	IPipeline(MCreatePipelineInfo info) {}
	virtual ~IPipeline() {}
};