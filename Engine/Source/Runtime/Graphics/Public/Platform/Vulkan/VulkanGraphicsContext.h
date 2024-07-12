#pragma once
#include "GraphicsContext.h"
#ifdef MT_PLATFORM_WINDOWS
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#include <volk.h>
#include <DSA\Dictionary.h>

namespace MarkTech
{
	class VulkanGraphisContext : public GraphicsContext
	{
	public:
		VulkanGraphisContext(Window* window);
		~VulkanGraphisContext();
		virtual void Destroy() override;

	private:
		VkInstance m_vkInstance;
	};
}