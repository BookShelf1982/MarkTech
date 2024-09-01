#include "2DRenderer.h"

namespace MarkTech
{
    Renderer2D InitRenderer2D()
    {
        Renderer2D renderer = {};

        GraphicsContextCreateInfo info = {};
        info.appInfo.pAppName = "SandboxApp";
        info.appInfo.majorVersion = 1;
        info.appInfo.minorVersion = 0;
        info.appInfo.patchVersion = 0;
        info.flags = GRAPHICS_CONTEXT_FLAGS_USE_WINDOW;
#ifdef DEBUG
        info.flags |= GRAPHICS_CONTEXT_FLAGS_DEBUG_MESSAGES;
#endif

        renderer.context = CreateGraphicsContext(&info);

        return renderer;
    }

    void ShutdownRenderer2D(Renderer2D* pRenderer)
    {
        DestroyGraphicsContext(&pRenderer->context);
    }
}