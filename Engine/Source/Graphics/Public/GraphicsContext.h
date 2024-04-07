#pragma once
#include "Core.h"
#include "Buffer.h"
#include "Shader.h"

enum class EGraphicsAPI
{
	OpenGL
};

class IGraphicsContext
{
public:
	IGraphicsContext() {}
	IGraphicsContext(EGraphicsAPI api) {}
	virtual ~IGraphicsContext() {}

	virtual void Test() = 0;
	virtual void SwapImages() = 0;
};