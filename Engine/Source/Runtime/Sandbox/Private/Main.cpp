#ifdef MT_PLATFORM_WINDOWS
#include <Windows.h>
#endif
#ifdef DEBUG
#include <crtdbg.h>
#endif

#include <GraphicsInterface.h>

using namespace MarkTech;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
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
	DestroyGraphicsContext(context);

#ifdef DEBUG
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}
