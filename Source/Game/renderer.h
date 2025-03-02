#pragma once

#include <stdint.h>
#include "window.h"
#include "resource_manager.h"
#include "math3D.h"

#include "data_structures.h"

#define MAX_SHADERS 4

struct SceneSnapshot 
{
	Array<uint32_t> models;
	Array<vec3f_t> origins;
};

struct RendererSettings
{
	Window* window;
	uint32_t flags;
};

struct Renderer {
	uintptr_t dc;
	uintptr_t context;
	uint32_t vao;
	uint32_t ppo;
	uint32_t ubo;
	Window* window;
	FixedArray<uint32_t, MAX_SHADERS> shaders;
};

enum BinaryType {
	BINARY_TYPE_SPIRV,
	BINARY_TYPE_COMPILED
};

struct LoadShaderInfo {
	size_t binarySize;
	char* binary;
	BinaryType binaryType;
};

void CreateRenderer(const RendererSettings& settings, Renderer& renderer);
void DestroyRenderer(Renderer& renderer);
bool LoadShader(Renderer& renderer, const LoadShaderInfo& info, uint32_t* id); // for compiled shader binaries
uint32_t LoadCompileShader(Renderer& renderer, const LoadShaderInfo& info); // for spirv
void CreatePipeline(Renderer& renderer);
void RenderFrame(Renderer& renderer, SceneSnapshot& snapshot);
void PresentFrame(Renderer& renderer);

void SetPostLoadFns(ResourceManager& manager, Renderer& renderer);