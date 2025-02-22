#ifndef RENDERER_H
#define RENDERER_H

#include <stdint.h>
#include "window.h"
#include "hash_table.h"

#define MAX_SHADERS 4

typedef struct {
	uint32_t vertShader;
	uint32_t fragShader;
	uint32_t program;
} ProgramPipeline;

DECLARE_HASHTABLE(uint32_t, ProgramPipeline);

typedef struct {
	uintptr_t dc;
	uintptr_t context;
	Window* window;
	uint32_t vao;
	uint32_t_ProgramPipeline_ht shaderTable;
} Renderer;

void CreateRenderer(Renderer* renderer, Window* window, uint32_t flags);
void DestroyRenderer(Renderer* renderer);
void LoadShaders(Renderer* renderer);
void RenderFrame(Renderer* renderer);

#endif // RENDERER_H
