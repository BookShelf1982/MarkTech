#include "VulkanRenderer.h"
#include <vector>


bool CVulkanRenderer::InitRenderer(IWindow* window)
{
    // -- instance creation -- //

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.apiVersion = VK_VERSION_1_3;
    appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
    appInfo.pApplicationName = "MarkTech Vulkan";
    appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
    appInfo.pEngineName = "MarkTech";

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = 0;
    createInfo.enabledLayerCount = 0;

#ifdef DEBUG
    const char* extensions[1] = { VK_EXT_DEBUG_UTILS_EXTENSION_NAME };
    const char* validationLayers[1] = { "VK_LAYER_KHRONOS_validation" };
    createInfo.enabledLayerCount = 1;
    createInfo.enabledExtensionCount = 1;
    createInfo.ppEnabledLayerNames = validationLayers;
    createInfo.ppEnabledExtensionNames = extensions;
#endif

    VkResult result = vkCreateInstance(&createInfo, nullptr, &m_vkInstance);

    if (vkFAILED(result))
        return false;

    // -- Debug layer -- //
#ifdef DEBUG
    CreateDebugMessenger(m_vkInstance, &m_vkDebugMessenger);
#endif

    // -- Physical device selection -- //
    uint32_t deviceCount = 0;
    result = vkEnumeratePhysicalDevices(m_vkInstance, &deviceCount, nullptr);

    if (vkFAILED(result) || deviceCount == 0)
        return false;

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_vkInstance, &deviceCount, devices.data());

    for (size_t i = 0; i < devices.size(); i++)
    {
        if (IsDeviceSuitable(devices[i]))
        {
            m_vkPhysicalDevice = devices[i];
            break;
        }
    }

    return true;
}

void CVulkanRenderer::ShutdownRenderer()
{
#ifdef DEBUG
    DestroyDebugMessenger(m_vkInstance, m_vkDebugMessenger);
#endif

    vkDestroyInstance(m_vkInstance, nullptr);
}

void CVulkanRenderer::CreateDebugMessenger(VkInstance instance, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
    VkDebugUtilsMessengerCreateInfoEXT debugInfo = {};
    debugInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debugInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debugInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debugInfo.pfnUserCallback = debugCallback;

    auto pFunc = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_vkInstance, "vkCreateDebugUtilsMessengerEXT");
    if (pFunc != nullptr)
        pFunc(m_vkInstance, &debugInfo, nullptr, pDebugMessenger);
}

void CVulkanRenderer::DestroyDebugMessenger(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_vkInstance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) 
        func(instance, debugMessenger, nullptr);
}

bool CVulkanRenderer::IsDeviceSuitable(VkPhysicalDevice device)
{
    MQueueFamilyIndices indices = FindQueueFamilies(device);

    return indices.IsComplete();
}

MQueueFamilyIndices CVulkanRenderer::FindQueueFamilies(VkPhysicalDevice device)
{
    MQueueFamilyIndices indices;
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int index = 0;
    for (size_t i = 0; i < queueFamilies.size(); i++) 
    {
        if (indices.IsComplete())
            break;

        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphicsFamily = index;
        }
        index++;
    }

    return indices;
}

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{   
    char buffer[2048];
    sprintf(buffer, "%s\n", pCallbackData->pMessage);
    OutputDebugStringA(buffer);
    
    return VK_FALSE;
}

void CreateVulkanRenderer(IRenderer** ppRenderer)
{
    *ppRenderer = new CVulkanRenderer();
}
