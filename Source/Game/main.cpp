#include "window.h"
#include "renderer.h"
#include "resource_manager.h"
#include "gameplay.h"
#include "dev_console.h"
#include "dev_gui.h"

#include <FileSystem.h>
#include <string.h>
#include <stdio.h>

#ifdef DEBUG
#include <crtdbg.h>
#include <stdlib.h>
#endif // DEBUG

using namespace MarkTech;

DevData devDat;
ResourceManager manager;
GameWorld world;
Window window;
Renderer renderer;
bool isRunning;

void OnWindowClose()
{
	isRunning = false;
}

bool Startup()
{
	isRunning = true;
	CreateWindow("Maze Walker", 1280, 720, window);
	SetCloseCallback(window, OnWindowClose);

	RendererSettings settings = {};
	settings.window = &window;
	settings.flags = 0x1;

	CreateRenderer(settings, renderer);
	SetPostLoadFns(manager, renderer);

	InitImGui(window);

	return true;
}

int Shutdown()
{
	AlignedFree(world.snapshot.models.elements);
	AlignedFree(world.snapshot.origins.elements);

	ShutdownImGui();
	DestroyRenderer(renderer);
	ReleaseWindow(window);
	FreeResources(manager);
	return 0;
}

int main()
{
#ifdef DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	if (!Startup())
		return 1;

	uint32_t obj = CreateMeshObject(world);
	world.meshes[obj].origin = { 1.0f, 0.0f, 0.0f, 0.0f };

	uint32_t vert = 0;
	uint32_t frag = 0;
	if (!LoadResource(manager, "vert.cache", &vert))
		LoadResource(manager, "vert.spv", &vert);
	if (!LoadResource(manager, "frag.cache", &frag))
		LoadResource(manager, "frag.spv", &frag);

	CreatePipeline(renderer);

	while (isRunning)
	{
		PollWindowEvents(window);
		if (!isRunning)
			break;

		TickGameWorld(world, 0.0025f);

		RenderFrame(renderer, world.snapshot);
		NewFrameImGui();

		bool b = true;
		ImGui::Begin("Viewer");
		ImGui::InputFloat2("Pos", world.meshes[0].origin.v);
		ImGui::End();

		RenderFrameImGui();
		PresentFrame(renderer);
	}

	return Shutdown();
}