#include "window.h"
#include "renderer.h"
#include "hash_table.h"

#ifdef DEBUG
#include <crtdbg.h>
#include <stdlib.h>
#endif // DEBUG

uint8_t g_isRunning = 1;

void on_close()
{
	g_isRunning = 0;
}

int main()
{
#ifdef DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	Window window;
	CreateWindow("Maze Walker", 800, 600, &window);
	SetCloseCallback(&window, on_close);

	Renderer renderer;
	CreateRenderer(&renderer, &window, 0);
	LoadShaders(&renderer);

	while (1)
	{
		PollWindowEvents(&window);
		if (g_isRunning != 1)
			break;

		RenderFrame(&renderer);
	}

	DestroyRenderer(&renderer);
	ReleaseWindow(&window);
	return 0;
}