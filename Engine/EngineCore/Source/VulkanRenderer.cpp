#include "VulkanRenderer.h"
#include <set>
#include <string>
#include <algorithm>

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

    std::vector<const char*> extensions;
    std::vector<const char*> validationLayers;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    extensions.emplace_back("VK_KHR_surface");

#ifdef MT_PLATFORM_WINDOWS
    extensions.emplace_back("VK_KHR_win32_surface");
#endif

#ifdef DEBUG
    extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    validationLayers.emplace_back("VK_LAYER_KHRONOS_validation");
    createInfo.enabledLayerCount = (uint32_t)validationLayers.size();
    createInfo.enabledExtensionCount = (uint32_t)extensions.size();
    createInfo.ppEnabledLayerNames = validationLayers.data();
    createInfo.ppEnabledExtensionNames = extensions.data();
#endif

    VkResult result = vkCreateInstance(&createInfo, nullptr, &m_vkInstance);

    if (vkFAILED(result))
        return false;

    // -- Debug layer -- //
#ifdef DEBUG
    CreateDebugMessenger(m_vkInstance, &m_vkDebugMessenger);
#endif

    // -- Window creation -- //
#ifdef MT_PLATFORM_WINDOWS
    VkWin32SurfaceCreateInfoKHR windowInfo = {};
    windowInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    windowInfo.hwnd = reinterpret_cast<CWinWindow*>(window)->GetHWND();
    windowInfo.hinstance = GetModuleHandleW(NULL);

    result = vkCreateWin32SurfaceKHR(m_vkInstance, &windowInfo, nullptr, &m_vkWindowSurface);
    if (vkFAILED(result))
        return false;
#endif

    // -- Physical device selection -- //
    uint32_t deviceCount = 0;
    result = vkEnumeratePhysicalDevices(m_vkInstance, &deviceCount, nullptr);

    if (vkFAILED(result) || deviceCount == 0)
        return false;

    std::vector<VkPhysicalDevice> devices(deviceCount);
    std::vector<const char*> requiredExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
    vkEnumeratePhysicalDevices(m_vkInstance, &deviceCount, devices.data());

    for (size_t i = 0; i < devices.size(); i++)
    {
        if (IsDeviceSuitable(devices[i], requiredExtensions))
        {
            m_vkPhysicalDevice = devices[i];
            break;
        }
    }

    // -- Logical Device Creation -- //
    MQueueFamilyIndices indices = FindQueueFamilies(m_vkPhysicalDevice);
    VkPhysicalDeviceFeatures deviceFeatures = {};

    std::vector<VkDeviceQueueCreateInfo> deviceQueueInfos;
    std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) 
    {
        VkDeviceQueueCreateInfo deviceQueueCreateInfo = {};
        deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        deviceQueueCreateInfo.queueFamilyIndex = queueFamily;
        deviceQueueCreateInfo.queueCount = 1;
        deviceQueueCreateInfo.pQueuePriorities = &queuePriority;
        deviceQueueInfos.push_back(deviceQueueCreateInfo);
    }

    VkDeviceCreateInfo deviceInfo = {};
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.pQueueCreateInfos = deviceQueueInfos.data();
    deviceInfo.queueCreateInfoCount = (uint32_t)deviceQueueInfos.size();
    deviceInfo.pEnabledFeatures = &deviceFeatures;
    deviceInfo.enabledExtensionCount = (uint32_t)requiredExtensions.size();
    deviceInfo.ppEnabledExtensionNames = requiredExtensions.data();

#ifdef DEBUG
    deviceInfo.enabledLayerCount = (uint32_t)validationLayers.size();
    deviceInfo.ppEnabledLayerNames = validationLayers.data();
#else
    deviceInfo.enabledLayerCount = 0;
    deviceInfo.enabledExtensionCount = 0;
#endif

    result = vkCreateDevice(m_vkPhysicalDevice, &deviceInfo, nullptr, &m_vkDevice);
    if (vkFAILED(result))
        return false;

    vkGetDeviceQueue(m_vkDevice, indices.graphicsFamily.value(), 0, &m_vkGraphicsRenderQueue);
    vkGetDeviceQueue(m_vkDevice, indices.presentFamily.value(), 0, &m_vkPresentQueue);

    // -- Swapchain creation -- //
    CreateSwapChain(m_vkDevice, &m_vkSwapchain, indices, window);

    // --  Image view creation -- //
    CreateImageViews();

    return true;
}

void CVulkanRenderer::ShutdownRenderer()
{
    for (size_t i = 0; i < m_vkSwapchainImageViews.size(); i++)
    {
        vkDestroyImageView(m_vkDevice, m_vkSwapchainImageViews[i], nullptr);
    }

    vkDestroySwapchainKHR(m_vkDevice, m_vkSwapchain, nullptr);

    vkDestroySurfaceKHR(m_vkInstance, m_vkWindowSurface, nullptr);

    vkDestroyDevice(m_vkDevice, nullptr);

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

bool CVulkanRenderer::CheckDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char*>& extensions)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(extensions.begin(), extensions.end());

    for (const auto& extension : availableExtensions)
    {
        size_t value = requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}


bool CVulkanRenderer::IsDeviceSuitable(VkPhysicalDevice device, const std::vector<const char*>& extensions)
{
    MQueueFamilyIndices indices = FindQueueFamilies(device); // Has required queue families
    bool hasExtensions = CheckDeviceExtensionSupport(device, extensions); // Has required extensions
    bool swapChainSupport = false;
    if (hasExtensions)
    {
        MSwapChainDetails details = GetSwapChainDetails(device);
        swapChainSupport = !details.formats.empty() && !details.presentModes.empty() && swapChainSupport;
    }

    return indices.IsComplete() && hasExtensions;
}

MSwapChainDetails CVulkanRenderer::GetSwapChainDetails(VkPhysicalDevice device)
{
    MSwapChainDetails details = {};
    VkSurfaceCapabilitiesKHR surfaceCapabilities;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_vkWindowSurface, &surfaceCapabilities);

    details.surfaceCapabilities = surfaceCapabilities;

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_vkWindowSurface, &formatCount, nullptr);

    if (formatCount != 0) 
    {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_vkWindowSurface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_vkWindowSurface, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_vkWindowSurface, &presentModeCount, details.presentModes.data());
    }

    return details;
}

MQueueFamilyIndices CVulkanRenderer::FindQueueFamilies(VkPhysicalDevice device)
{
    MQueueFamilyIndices indices;
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    VkBool32 presentSupport = false;
    int index = 0;

    for (size_t i = 0; i < queueFamilies.size(); i++) 
    {
        if (indices.IsComplete())
            break;

        vkGetPhysicalDeviceSurfaceSupportKHR(device, index, m_vkWindowSurface, &presentSupport);

        if (presentSupport)
            indices.presentFamily = index;

        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphicsFamily = index;
        }
        index++;
    }

    return indices;
}

VkSurfaceFormatKHR CVulkanRenderer::ChooseSwapChainFormat(const std::vector<VkSurfaceFormatKHR>& formats)
{
    for (size_t i = 0; i < formats.size(); i++)
    {
        if (formats[i].format == VK_FORMAT_B8G8R8A8_SRGB && formats[i].colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR)
        {
            return formats[i];
        }
    }

    return formats[0];
}

VkPresentModeKHR CVulkanRenderer::ChooseSwapChainPresentMode(const std::vector<VkPresentModeKHR> modes)
{
    return VK_PRESENT_MODE_IMMEDIATE_KHR; // YSYNC OFF
}

bool CVulkanRenderer::CreateSwapChain(VkDevice device, VkSwapchainKHR* swpachain, MQueueFamilyIndices indices, IWindow* pWindow)
{
    MSwapChainDetails details = GetSwapChainDetails(m_vkPhysicalDevice);
    VkSurfaceFormatKHR surfaceFormat = ChooseSwapChainFormat(details.formats);
    VkPresentModeKHR mode = ChooseSwapChainPresentMode(details.presentModes);
    VkExtent2D extent = ChooseSwapChainExtent2D(details.surfaceCapabilities, pWindow);
    uint32_t imageCount = details.surfaceCapabilities.minImageCount + 1;

    VkSwapchainCreateInfoKHR swapChainInfo = {};
    swapChainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapChainInfo.surface = m_vkWindowSurface;
    swapChainInfo.minImageCount = imageCount;
    swapChainInfo.imageFormat = surfaceFormat.format;
    swapChainInfo.imageColorSpace = surfaceFormat.colorSpace;
    swapChainInfo.imageExtent = extent;
    swapChainInfo.imageArrayLayers = 1;
    swapChainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    if (indices.graphicsFamily != indices.presentFamily) {
        swapChainInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapChainInfo.queueFamilyIndexCount = 2;
        swapChainInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else {
        swapChainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapChainInfo.queueFamilyIndexCount = 0; // Optional
        swapChainInfo.pQueueFamilyIndices = nullptr; // Optional
    }

    swapChainInfo.preTransform = details.surfaceCapabilities.currentTransform;
    swapChainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapChainInfo.presentMode = mode;
    swapChainInfo.clipped = VK_TRUE;
    swapChainInfo.oldSwapchain = VK_NULL_HANDLE;

    VkResult result = vkCreateSwapchainKHR(device, &swapChainInfo, nullptr, swpachain);
    if (vkFAILED(result))
        return false;

    uint32_t swapchainImageCount = 0;
    vkGetSwapchainImagesKHR(device, *swpachain, &swapchainImageCount, nullptr);
    m_vkSwapchainImages.resize(swapchainImageCount);
    vkGetSwapchainImagesKHR(device, *swpachain, &swapchainImageCount, m_vkSwapchainImages.data());

    m_vkSwapchainExtent = extent;
    m_vkSwapchainImageFormat = surfaceFormat.format;

    return true;
}

VkExtent2D CVulkanRenderer::ChooseSwapChainExtent2D(const VkSurfaceCapabilitiesKHR& capabilities, IWindow* pWindow)
{
    int width = 0;
    int height = 0;
    
#ifdef MT_PLATFORM_WINDOWS
    CWinWindow* pWinWindow = reinterpret_cast<CWinWindow*>(pWindow);
    width = pWinWindow->GetWidth();
    height = pWinWindow->GetHeight();
#endif

    VkExtent2D actualExtent = {
        static_cast<uint32_t>(width),
        static_cast<uint32_t>(height)
    };
    
    actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
    
    return actualExtent;
}

void CVulkanRenderer::CreateImageViews()
{
    m_vkSwapchainImageViews.resize(m_vkSwapchainImages.size());
    
    for (size_t i = 0; i < m_vkSwapchainImages.size(); i++) 
    {
        VkImageViewCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = m_vkSwapchainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = m_vkSwapchainImageFormat;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        VkResult result = vkCreateImageView(m_vkDevice, &createInfo, nullptr, &m_vkSwapchainImageViews[i]);
    }
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
