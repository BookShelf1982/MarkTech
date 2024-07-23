#include "GraphicsInterface.h"
#include <Version.h>
#include <Log.h>
#include <MemoryArena.h>

namespace MarkTech
{
    VkDebugUtilsMessengerEXT gvkDebugMessenger = VK_NULL_HANDLE;

    static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData) {

        DebugPrintF("%s\n", pCallbackData->pMessage);

        return VK_FALSE;
    }

    bool SetupDebugCallback(VulkanGraphicsContext* pContext)
    {
        VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = DebugCallback;
        createInfo.pUserData = nullptr; // Optional

        if (vkCreateDebugUtilsMessengerEXT(pContext->m_vkInstance, &createInfo, nullptr, &gvkDebugMessenger) != VK_SUCCESS)
            return false;

        return true;
    }

    void DestroyDebugCallback(VulkanGraphicsContext* pContext)
    {
        vkDestroyDebugUtilsMessengerEXT(pContext->m_vkInstance, gvkDebugMessenger, nullptr);
    }

    VulkanGraphicsContext CreateGraphicsContext(ContextSettings settings)
    {
        VulkanGraphicsContext context = {};

        volkInitialize(); // Load vulkan

        // App info
        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.apiVersion = VK_API_VERSION_1_2;
        appInfo.pApplicationName = settings.pAppName;
        appInfo.applicationVersion = VK_MAKE_VERSION(settings.majorVersion, settings.minorVersion, settings.patchVersion);
        appInfo.pEngineName = "MarkTech";
        appInfo.engineVersion = VK_MAKE_VERSION(MT_MAJOR_VERSION, MT_MINOR_VERSION, MT_PATCH_VERSION);

        
        // Extension bitflags
        U32 extensionCount = 0;
        char* ppExtensions[8];

        if (settings.extensionBitflags & EXT_DEBUG)
        {
            ppExtensions[extensionCount] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
            extensionCount++;
        }

        // Create vulkan instance
        VkInstanceCreateInfo instanceInfo = {};
        instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceInfo.pApplicationInfo = &appInfo;
        if (extensionCount > 0)
        {
            instanceInfo.ppEnabledExtensionNames = ppExtensions;
            instanceInfo.enabledExtensionCount = extensionCount;
        }

        if (vkCreateInstance(&instanceInfo, nullptr, &context.m_vkInstance) != VK_SUCCESS)
            context.m_vkInstance = VK_NULL_HANDLE;
        else
            volkLoadInstance(context.m_vkInstance);

        return context;
    }

    void DestroyGraphicsContext(VulkanGraphicsContext* pContext)
    {
        vkDestroyInstance(pContext->m_vkInstance, nullptr);
        volkFinalize(); // unload vulkan
    }

    void RankAllGraphicsDevices(VulkanGraphicsContext* pContext, U32* pSize, U32* pScores)
    {
        if (!pSize)
            return;

        // if pScores is nullptr then just give user the number of devices
        if (!pScores)
        {
            vkEnumeratePhysicalDevices(pContext->m_vkInstance, pSize, nullptr);
            return;
        }

        VkPhysicalDevice devices[MT_MAX_GRAPHICS_DEVICES] = {};
        vkEnumeratePhysicalDevices(pContext->m_vkInstance, pSize, nullptr);
        vkEnumeratePhysicalDevices(pContext->m_vkInstance, pSize, devices);

        for (U32 i = 0; i < *pSize; i++)
        {
            U32 deviceScore = 0;
            VkPhysicalDeviceProperties props;
            //VkPhysicalDeviceFeatures features;
            vkGetPhysicalDeviceProperties(devices[i], &props);

            // Rank devices on thier properties and features
            deviceScore += props.limits.maxColorAttachments;
            deviceScore += props.limits.maxDescriptorSetSamplers;
            deviceScore += props.limits.maxBoundDescriptorSets;
            deviceScore += props.limits.maxFramebufferWidth;
            deviceScore += props.limits.maxFramebufferHeight;

            pScores[i] = deviceScore;
        }
    }

    VulkanPhysicalDevice ChooseBestGraphicsDevice(VulkanGraphicsContext* pContext)
    {
        U32 size = 0;
        VkPhysicalDevice devices[MT_MAX_GRAPHICS_DEVICES] = {};
        vkEnumeratePhysicalDevices(pContext->m_vkInstance, &size, nullptr);
        vkEnumeratePhysicalDevices(pContext->m_vkInstance, &size, devices);

        U32 bestDeviceIndex = 0;
        U32 bestDeviceScore = 0;
        for (U32 i = 0; i < size; i++)
        {
            U32 deviceScore = 0;
            VkPhysicalDeviceProperties props;
            //VkPhysicalDeviceFeatures features;
            vkGetPhysicalDeviceProperties(devices[i], &props);

            // Rank devices on thier properties and features
            deviceScore += props.limits.maxColorAttachments;
            deviceScore += props.limits.maxDescriptorSetSamplers;
            deviceScore += props.limits.maxBoundDescriptorSets;
            deviceScore += props.limits.maxFramebufferWidth;
            deviceScore += props.limits.maxFramebufferHeight;

            if (deviceScore > bestDeviceScore)
            {
                bestDeviceIndex = i;
                bestDeviceScore = bestDeviceScore;
            }
        }

        VulkanPhysicalDevice device = {};
        device.m_vkPhysicalDevice = devices[bestDeviceIndex];
        return device;
    }

    VulkanGraphicsDevice CreateGraphicsDevice(VulkanPhysicalDevice* pPhysicalDevice)
    {
        VulkanGraphicsDevice device = {};
        VkDeviceCreateInfo deviceInfo = {};
        deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        //deviceInfo.

        //vkCreateDevice(pPhysicalDevice->m_vkPhysicalDevice, &deviceInfo, nullptr, &device.m_vkDevice);
        return device;
    }

    void DestroyGraphicsDevice(VulkanGraphicsDevice* pDevice)
    {
        vkDestroyDevice(pDevice->m_vkDevice, nullptr);
    }
    
    VulkanQueueFamiles FindQueueFamilies(VulkanPhysicalDevice* pPhysicalDevice)
    {
        VulkanQueueFamiles familes = {};
        VkQueueFamilyProperties props[MT_MAX_QUEUE_FAMILIES] = {};
        vkGetPhysicalDeviceQueueFamilyProperties(pPhysicalDevice->m_vkPhysicalDevice, nullptr, props);

        return familes;
    }
}