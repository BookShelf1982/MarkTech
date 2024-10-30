#include "GraphicsInterface.h"
#include "Vulkan\VulkanContext.h"

namespace MarkTech
{
	GraphicsAPI gAPI;

	ResultCode CreateGraphicsContext(const CreateGraphicsContextInfo& info, GraphicsContext* pContext)
	{
		switch (info.api)
		{
		case GRAPHICS_API_VULKAN:
		{
			gAPI = GRAPHICS_API_VULKAN;
			VulkanContextInfo vulkanInfo;
			vulkanInfo.flags = info.flags;
			VulkanApplicationInfo appInfo;
			appInfo.pName = info.pAppInfo->pName;
			appInfo.verMajor = info.pAppInfo->verMajor;
			appInfo.verMinor = info.pAppInfo->verMinor;
			appInfo.verPatch = info.pAppInfo->verPatch;
			vulkanInfo.pAppInfo = &appInfo;
			return (ResultCode)CreateVulkanContext(vulkanInfo, (VulkanContext**)pContext);
		} break;
		}

		return RC_FAILED;
	}

	void DestroyGraphicsContext(GraphicsContext context)
	{
		switch (gAPI)
		{
		case GRAPHICS_API_VULKAN:
		{
			DestroyVulkanContext((VulkanContext*)context);
		} break;
		}
	}
}