#ifdef MT_PLATFORM_WINDOWS
#include <Windows.h>
#endif

#include <stdio.h>

/*
* Objective: Make pac man
*/

bool g_isClosing = false;
double g_clockFrequency = 0;
const unsigned long long g_tickLength = 15;

void StartTime()
{
	LARGE_INTEGER integer;
	QueryPerformanceFrequency(&integer);
	g_clockFrequency = (double)integer.QuadPart / 1000.0;
}

double GetTime()
{
	LARGE_INTEGER integer;
	QueryPerformanceCounter(&integer);
	return (double)integer.QuadPart / g_clockFrequency;
}

#define BUTTON_UP 0x1
#define BUTTON_DOWN 0x2
#define BUTTON_LEFT 0x4
#define BUTTON_RIGHT 0x8
#define BUTTON_SELECT 0x10

struct InputState 
{
	unsigned char buttons;
};

InputState g_inputState = {};

#define WNDCLASS_NAME L"PACMANWND"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
	{
		g_isClosing = true;
	} return 0;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	{
		switch (wParam)
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
	} return 0;
	case WM_KEYUP:
	case WM_SYSKEYUP:
	{
		switch (wParam)
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
	} return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

HWND CreateWin32Window(HINSTANCE inst) 
{
	WNDCLASS windowClass = {};
	windowClass.hInstance = inst;
	windowClass.lpfnWndProc = WindowProc;
	windowClass.lpszClassName = WNDCLASS_NAME;
	RegisterClass(&windowClass);

	HWND hwnd = CreateWindowW(WNDCLASS_NAME, L"Pac-Man", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, NULL, NULL);
	ShowWindow(hwnd, SW_SHOW);
	return hwnd;
}

void DestroyWin32Window(HWND hwnd, HINSTANCE inst) 
{
	DestroyWindow(hwnd);
	UnregisterClassW(WNDCLASS_NAME, inst);
}

void Win32MessagePump(HWND hwnd)
{
	MSG msg;
	while (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

struct Entity
{
	double x, y;
};

struct GameState
{
	bool buttonDown;
	int rot;
	Entity player;
	Entity ghosts[5];
};

GameState g_gameState = {};
HANDLE g_isClosingMutex = NULL;

void TickGameState() 
{
	if (g_inputState.buttons & BUTTON_LEFT)
	{
		g_gameState.player.x += 0.001;
	}
	if (g_inputState.buttons & BUTTON_RIGHT)
	{
		g_gameState.player.x -= 0.001;
	}
	if (g_inputState.buttons & BUTTON_UP)
	{
		g_gameState.player.y += 0.001;
	}
	if (g_inputState.buttons & BUTTON_DOWN)
	{
		g_gameState.player.y -= 0.001;
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

unsigned int TickThread(void* p)
{
/*#ifdef DEBUG
	unsigned int count = 0;
#endif*/
	while (true)
	{
		double startTime = GetTime();

		WaitForSingleObject(g_isClosingMutex, INFINITE);
		const bool isClosing = g_isClosing;
		ReleaseMutex(g_isClosingMutex);
		if (isClosing)
			break;

		TickGameState();

		double endTime = GetTime();

		double elapsed = (endTime - startTime);
		if (elapsed < (double)g_tickLength)
			Sleep(g_tickLength - (int)elapsed);

/*#ifdef DEBUG
		char buffer[16] = "";
		sprintf_s(buffer, "Tick: %i\n", count);
		OutputDebugStringA(buffer);
		count++;
#endif*/
	}

	return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) 
{
	StartTime();
	g_isClosingMutex = CreateMutex(NULL, false, NULL);
	HANDLE tickThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TickThread, NULL, 0, NULL);
	HWND window = CreateWin32Window(hInstance);

	while (true) 
	{
		Win32MessagePump(window);
		
		WaitForSingleObject(g_isClosingMutex, INFINITE);
		const bool isClosing = g_isClosing;
		ReleaseMutex(g_isClosingMutex);

		if (isClosing)
			break;
	}

#ifdef DEBUG
	char buffer[128] = "";
	sprintf_s(buffer, "Player entity: Pos: {  %f,  %f } Rot: %i\n", g_gameState.player.x, g_gameState.player.y, g_gameState.rot);
	OutputDebugStringA(buffer);
#endif

	DestroyWin32Window(window, hInstance);

	WaitForSingleObject(tickThread, INFINITE);
	CloseHandle(tickThread);
	CloseHandle(g_isClosingMutex);
	return 0;
}