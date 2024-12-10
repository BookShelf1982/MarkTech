#ifdef MT_PLATFORM_WINDOWS
#include <Windows.h>
#endif

#include "PrimitiveTypes\intalias.h"
#include "Time\Time.h"
#include "Threads\Thread.h"
#include "Threads\Mutex.h"
#include "Window\Window.h"

#include <stdio.h>

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
	double x, y;
};

struct GameState
{
	bool buttonDown;
	I32 rot;
	Entity player;
	Entity ghosts[5];
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

void TickGameState() 
{
	if (g_inputState.buttons & BUTTON_LEFT)
	{
		g_gameState.player.x += 1;
	}
	if (g_inputState.buttons & BUTTON_RIGHT)
	{
		g_gameState.player.x -= 1;
	}
	if (g_inputState.buttons & BUTTON_UP)
	{
		g_gameState.player.y += 1;
	}
	if (g_inputState.buttons & BUTTON_DOWN)
	{
		g_gameState.player.y -= 1;
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
}

unsigned int TickThread()
{
	while (true)
	{
		double startTime = GetTime();

		AcquireMutex(g_isClosingMutex);
		const bool isClosing = g_isClosing;
		FreeMutex(g_isClosingMutex);
		if (isClosing)
			break;

		TickGameState();

		double endTime = GetTime();

		double elapsed = (endTime - startTime);
		if (elapsed < (double)g_tickLength)
			Sleep(g_tickLength - (int)elapsed);
	}

	return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) 
{
	StartTime();
	g_isClosingMutex = MakeMutex();
	Thread tickThread = StartThread(TickThread);

	WindowProps props;
	props.width = 800;
	props.height = 600;
	props.pTitle = "Pac-Man";
	props.style = WINDOW_STYLE_WINDOWED;
	props.eventHandler = WindowHandler;
	Window window = MakeWindow(props);

	while (true) 
	{
		PollWindowEvents(window);
		
		AcquireMutex(g_isClosingMutex);
		const bool isClosing = g_isClosing;
		FreeMutex(g_isClosingMutex);

		if (isClosing)
			break;
	}

#ifdef DEBUG
	char buffer[64] = "";
	sprintf_s(buffer, "Player entity: Pos: {  %f,  %f } Rot: %i\n", g_gameState.player.x, g_gameState.player.y, g_gameState.rot);
	OutputDebugStringA(buffer);
#endif

	KillWindow(window);

	JoinThread(tickThread);
	DestroyThread(tickThread);
	DestoryMutex(g_isClosingMutex);
	return 0;
}