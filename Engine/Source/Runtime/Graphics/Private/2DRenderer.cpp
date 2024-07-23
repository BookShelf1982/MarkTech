#include "2DRenderer.h"

namespace MarkTech
{
    VulkanGraphicsContext gContext = {};
    VulkanGraphicsDevice gDevice = {};

    bool InitRenderer2D()
    {
        // Graphics Creation
        ContextSettings settings = {};
        settings.majorVersion = 1;
        settings.minorVersion = 0;
        settings.patchVersion = 0;
        settings.pAppName = "Game";
        settings.extensionBitflags = EXT_DEBUG;

        gContext = CreateGraphicsContext(settings); // Create context
#ifdef DEBUG
        SetupDebugCallback(&gContext);
#endif
        // Choose the best device
        VulkanPhysicalDevice physicalDevice = ChooseBestGraphicsDevice(&gContext);
        VulkanGraphicsDevice device = CreateGraphicsDevice(&physicalDevice);

        return true;
    }

    void ShutdownRenderer2D()
    {
        // Cleanup
        DestroyGraphicsDevice(&gDevice);
#ifdef DEBUG
        DestroyDebugCallback(&gContext);
#endif
        DestroyGraphicsContext(&gContext);
    }
}