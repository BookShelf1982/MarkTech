#ifdef MT_PLATFORM_WINDOWS
#include <Windows.h>
#endif
#ifdef DEBUG
#include <crtdbg.h>
#endif

#include <Window.h>
#include <GraphicsInterface.h>

using namespace MarkTech;

bool gIsRunning = true;

void WindowEventHandler(void* pEvent)
{
	WindowEventType event = (WindowEventType) *(WindowEventType*)pEvent;
	switch (event)
	{
	case WINDOW_EVENT_CLOSE:
	{
		gIsRunning = false;
	} return;
	}
}

void SubWindowEventHandler(void* pEvent)
{
	return;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	Window window;
	WindowInfo windowInfo;
	windowInfo.defaultMode = WINDOW_MODE_WINDOWED;
	windowInfo.pTitle = "MarkTech Window";
	windowInfo.width = 640;
	windowInfo.height = 480;
	windowInfo.pfnEventHandler = WindowEventHandler;

	ConstructWindow(windowInfo, window);

	Window subWindow;
	WindowInfo subWindowInfo;
	subWindowInfo.defaultMode = WINDOW_MODE_WINDOWED;
	subWindowInfo.pTitle = "Sub Window";
	subWindowInfo.width = 640;
	subWindowInfo.height = 480;
	subWindowInfo.pfnEventHandler = SubWindowEventHandler;

	ConstructWindow(subWindowInfo, subWindow);

	ApplicationInfo appInfo;
	appInfo.pName = "MarkTech";
	appInfo.verMajor = 1;
	appInfo.verMinor = 0;
	appInfo.verPatch = 0;

	CreateGraphicsContextInfo info;
	info.api = GRAPHICS_API_VULKAN;
	info.flags = 0;
	info.pAppInfo = &appInfo;
	GraphicsContext context;
#ifdef DEBUG
	info.flags |= CONTEXT_FLAGS_DEBUG_MESSAGES;
#endif

	CreateGraphicsContext(info, &context);

	while (gIsRunning)
	{
		PollWindowMessages();
	}

	DestroyGraphicsContext(context);
	ReleaseWindow(subWindow);
	ReleaseWindow(window);
	

#ifdef DEBUG
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}
