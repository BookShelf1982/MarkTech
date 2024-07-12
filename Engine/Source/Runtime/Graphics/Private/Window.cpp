#include "Window.h"
#ifdef MT_PLATFORM_WINDOWS
#include "Platform\Windows\WinWindow.h"
#endif
#include <Memory\MemoryManager.h>
#include <new>

namespace MarkTech
{
    Window* Window::New(WindowProperties properties)
    {
#ifdef MT_PLATFORM_WINDOWS
        return new (MemoryManager::Alloc(sizeof(WinWindow), 8)) WinWindow(properties);
#endif
    }
}