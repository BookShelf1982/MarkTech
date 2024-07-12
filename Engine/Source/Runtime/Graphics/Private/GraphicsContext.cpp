#include "GraphicsContext.h"
#include "Platform\Vulkan\VulkanGraphicsContext.h"
#include <Memory\MemoryManager.h>
#include <new>

namespace MarkTech
{
    U64 GraphicsContext::GetRquiredMemSize()
    {
        return sizeof(VulkanGraphisContext);
    }

    GraphicsContext* GraphicsContext::CreateContext(Window* window)
    {
        return new (MemoryManager::Alloc(sizeof(VulkanGraphisContext), 8)) VulkanGraphisContext(window);
    }
}