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
	windowInfo.defaultMode = WINDOW_MODE_BORDERLESS_WINDOWED;
	windowInfo.pTitle = "MarkTech Window";
	windowInfo.width = 640;
	windowInfo.height = 480;
	windowInfo.pfnEventHandler = WindowEventHandler;

	ConstructWindow(windowInfo, window);

	ApplicationInfo appInfo;
	appInfo.pName = "MarkTech";
	appInfo.verMajor = 1;
	appInfo.verMinor = 0;
	appInfo.verPatch = 0;

	GraphicsContextCreateInfo info;
	info.api = GRAPHICS_API_VULKAN;
	info.flags = 0;
	info.pAppInfo = &appInfo;
	GraphicsContext context = nullptr;
#ifdef DEBUG
	info.flags |= CONTEXT_FLAGS_DEBUG_MESSAGES;
#endif

	CreateGraphicsContext(info, &context);

	SwapchainCreateInfo swapchainInfo;
	swapchainInfo.pWindow = &window;
	swapchainInfo.oldSwapchain = nullptr;
	swapchainInfo.presentationMode = PRESENTATION_MODE_FIFO;

	Swapchain swapchain = nullptr;
	CreateSwapchain(context, swapchainInfo, &swapchain);

	while (gIsRunning)
	{
		PollWindowMessages();
	}

	DestroySwapchain(context, swapchain);
	DestroyGraphicsContext(context);
	ReleaseWindow(window);
	
#ifdef DEBUG
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}
