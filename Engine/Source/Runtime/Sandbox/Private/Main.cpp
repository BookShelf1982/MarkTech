#ifdef MT_PLATFORM_WINDOWS
#include <Windows.h>
#endif

#include "PrimitiveTypes\intalias.h"
#include "Time\Time.h"
#include "Threads\Thread.h"
#include "Threads\Mutex.h"
#include "Window\Window.h"
#include "Renderer\Renderer.h"
#include "Math3D\Math3D.h"

#include <stdio.h>
#ifdef DEBUG
#include <crtdbg.h>
#endif

/*
* Objective: Make pac man
*/

using namespace MarkTech;

bool g_isClosing = false;
const U64 g_tickLength = 15;

#define BUTTON_UP 0x1
#define BUTTON_DOWN 0x2
#define BUTTON_LEFT 0x4
#define BUTTON_RIGHT 0x8
#define BUTTON_SELECT 0x10

struct InputState 
{
	U8 buttons;
};

InputState g_inputState = {};

struct Entity
{
	Vector2 pos;
};

#define NUM_GHOST 5
#define ENTITY_RADIUS 2

struct GameState
{
	bool buttonDown;
	I32 rot;
	Vector2 wishDir;
	Entity player;
	Entity ghosts[NUM_GHOST];
};

GameState g_gameState = {};
Mutex g_isClosingMutex;

void WindowHandler(WindowEventHandle wndEvent)
{
	WindowEventType type = *(WindowEventType*)wndEvent;
	switch (type)
	{
	case WINDOW_EVENT_TYPE_CLOSE:
		g_isClosing = true;
		return;
	case WINDOW_EVENT_TYPE_KEYDOWN:
	{
		WindowKeyChangedEvent* pEvent = (WindowKeyChangedEvent*)wndEvent;
		switch (pEvent->keycode)
		{
		case VK_UP:
			g_inputState.buttons |= BUTTON_UP;
			break;
		case VK_DOWN:
			g_inputState.buttons |= BUTTON_DOWN;
			break;
		case VK_LEFT:
			g_inputState.buttons |= BUTTON_LEFT;
			break;
		case VK_RIGHT:
			g_inputState.buttons |= BUTTON_RIGHT;
			break;
		case VK_SPACE:
			g_inputState.buttons |= BUTTON_SELECT;
			break;
		case VK_ESCAPE:
			g_isClosing = true;
		}
	} return;
	case WINDOW_EVENT_TYPE_KEYUP:
	{
		WindowKeyChangedEvent* pEvent = (WindowKeyChangedEvent*)wndEvent;
		switch (pEvent->keycode)
		{
		case VK_UP:
			g_inputState.buttons = (g_inputState.buttons & ~(1 << (1 - 1)));
			break;
		case VK_DOWN:
			g_inputState.buttons = (g_inputState.buttons & ~(1 << (2 - 1)));
			break;
		case VK_LEFT:
			g_inputState.buttons = (g_inputState.buttons & ~(1 << (3 - 1)));
			break;
		case VK_RIGHT:
			g_inputState.buttons = (g_inputState.buttons & ~(1 << (4 - 1)));
			break;
		case VK_SPACE:
			g_inputState.buttons = (g_inputState.buttons & ~(1 << (5 - 1)));
			break;
		}
	} return;
	}
}

void TickPlayerMovement(float dt)
{
	g_gameState.wishDir = { 0.0f, 0.0f };
	if (g_inputState.buttons & BUTTON_RIGHT)
	{
		g_gameState.wishDir.x += 1.0f;
	}
	if (g_inputState.buttons & BUTTON_LEFT)
	{
		g_gameState.wishDir.x -= 1.0f;
	}
	if (g_inputState.buttons & BUTTON_UP)
	{
		g_gameState.wishDir.y += 1.0f;
	}
	if (g_inputState.buttons & BUTTON_DOWN)
	{
		g_gameState.wishDir.y -= 1.0f;
	}
	if (g_inputState.buttons & BUTTON_SELECT)
	{
		if (!g_gameState.buttonDown)
		{
			g_gameState.buttonDown = true;

			g_gameState.rot += 90;
			if (g_gameState.rot >= 360)
			{
				g_gameState.rot = 0;
			}
		}
	}
	else
	{
		g_gameState.buttonDown = false;
	}

	g_gameState.player.pos = AddV2(g_gameState.player.pos, MultiplyByScalarV2(MultiplyByScalarV2(NormalizeV2(g_gameState.wishDir), 2.0f), dt));
}

void TickCollisions()
{
	for (U32 i = 0; i < NUM_GHOST; i++)
	{
		bool xintersect = (g_gameState.player.pos.x - g_gameState.ghosts[i].pos.x) < ENTITY_RADIUS;
		bool yintersect = (g_gameState.player.pos.y - g_gameState.ghosts[i].pos.y) < ENTITY_RADIUS;
	}
}

void TickGameState(float dt) 
{
	TickPlayerMovement(dt);
	TickCollisions();
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) 
{
	StartTime();
	double lastTime = GetTime();

	WindowProps props;
	props.width = 800;
	props.height = 600;
	props.pTitle = "Pac-Man";
	props.style = WINDOW_STYLE_BORDERLESS;
	props.eventHandler = WindowHandler;
	Window window = MakeWindow(props);

	RendererConfig rConfig;
	rConfig.enableVSync = false;
	rConfig.pWindow = &window;
	rConfig.textureFiltering = TEXTURE_FILTERING_BILINEAR;

	Renderer renderer;
	if (!InitRenderer(rConfig, renderer))
	{ 
		WindowMessageBox(window, "MarkTech Error", "Failed to initialize renderer", WINDOW_MESSAGE_TYPE_ERROR);
		return 1;
	}

	while (true) 
	{
		double startTime = GetTime();
		PollWindowEvents(window);
		
		const bool isClosing = g_isClosing;
		if (isClosing)
			break;

		float dt = (float)(startTime - lastTime) / 1000.0f;

		TickGameState(dt);

		Matrix3x3 world = Translate3x3(g_gameState.player.pos.x, -g_gameState.player.pos.y) * Scale3x3(200.0f, 200.0f);
		Matrix3x3 projection = OrthoProjection3x3(0.0f, 1920.0f, 0.0f, 1080.0f);
		Matrix3x3 transform = world * projection;

		RenderFrame(renderer, transform);
		lastTime = startTime;
	}

#ifdef DEBUG
	char buffer[64] = "";
	sprintf_s(buffer, "Player entity: Pos: {  %f,  %f } Rot: %i\n", g_gameState.player.pos.x, g_gameState.player.pos.y, g_gameState.rot);
	OutputDebugStringA(buffer);
#endif

	ShutdownRenderer(renderer);
	KillWindow(window);

	//JoinThread(tickThread);
	//DestroyThread(tickThread);
	//DestoryMutex(g_isClosingMutex);

#ifdef DEBUG
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}