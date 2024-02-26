#include "VulkanRenderer.h"
#include "AssetRegistry.h"
#include <array>
#include <set>
#include <string>
#include <algorithm>


CVulkanRenderer::CVulkanRenderer()
{
}

CVulkanRenderer::~CVulkanRenderer()
{
    delete[] m_pvkSwapchainFrameBuffers;
    delete[] m_pvkSwapchainImageViews;
    delete[] m_pvkSwapchainImages;
}

bool CVulkanRenderer::InitRenderer(IWindow* window)
{
    VkResult result = volkInitialize();

    // -- Instance creation -- //
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.apiVersion = VK_API_VERSION_1_2;
    appInfo.pNext = NULL;
    appInfo.applicationVersion = 1;
    appInfo.pApplicationName = "Unkown";
    appInfo.engineVersion = 1;
    appInfo.pEngineName = "No Engine";

    std::vector<const char*> extensions;
    std::vector<const char*> validationLayers;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.flags = 0;
    createInfo.pNext = NULL;

#ifdef USE_VALIDATION_LAYERS
    extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    validationLayers.emplace_back("VK_LAYER_KHRONOS_validation");
#endif

    createInfo.enabledLayerCount = (uint32_t)validationLayers.size();
    createInfo.ppEnabledLayerNames = validationLayers.data();

#ifdef MT_PLATFORM_WINDOWS
    extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
    extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);

    createInfo.enabledExtensionCount = (uint32_t)extensions.size();
    createInfo.ppEnabledExtensionNames = extensions.data();
#endif

    result = vkCreateInstance(&createInfo, nullptr, &m_vkInstance);

    if (vkFAILED(result))
        return false;

    volkLoadInstance(m_vkInstance);

    // -- Debug layer -- //
#ifdef USE_VALIDATION_LAYERS
    CreateDebugMessenger(m_vkInstance, &m_vkDebugMessenger);
#endif

    // -- Window creation -- //
    VkWin32SurfaceCreateInfoKHR windowInfo = {};
    windowInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    windowInfo.hwnd = reinterpret_cast<CWinWindow*>(window)->GetHWND();
    windowInfo.hinstance = GetModuleHandleW(NULL);

    PFN_vkCreateWin32SurfaceKHR pfnvkCreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR)vkGetInstanceProcAddr(m_vkInstance, "vkCreateWin32SurfaceKHR");
    result = pfnvkCreateWin32SurfaceKHR(m_vkInstance, &windowInfo, nullptr, &m_vkWindowSurface);
    if (vkFAILED(result))
        return false;

    // -- Physical device selection -- //
    uint32_t deviceCount = 0;
    result = vkEnumeratePhysicalDevices(m_vkInstance, &deviceCount, nullptr);

    if (vkFAILED(result) || deviceCount == 0)
        return false;

    std::vector<VkPhysicalDevice> devices(deviceCount);
    std::vector<const char*> requiredExtensions = {
        VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME,
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
    deviceFeatures.samplerAnisotropy = VK_TRUE;

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

#ifdef USE_VALIDATION_LAYERS
    deviceInfo.enabledLayerCount = (uint32_t)validationLayers.size();
    deviceInfo.ppEnabledLayerNames = validationLayers.data();
#else
    deviceInfo.enabledLayerCount = 0;
#endif

    result = vkCreateDevice(m_vkPhysicalDevice, &deviceInfo, nullptr, &m_vkDevice);
    if (vkFAILED(result))
        return false;

    vkGetDeviceQueue(m_vkDevice, indices.graphicsFamily.value(), 0, &m_vkGraphicsRenderQueue);
    vkGetDeviceQueue(m_vkDevice, indices.presentFamily.value(), 0, &m_vkPresentQueue);

    // -- Vulkan Memory Allocator -- //
    VmaVulkanFunctions vulkanFunctions{};
    vulkanFunctions.vkAllocateMemory = vkAllocateMemory;
    vulkanFunctions.vkBindBufferMemory = vkBindBufferMemory;
    vulkanFunctions.vkBindBufferMemory2KHR = vkBindBufferMemory2KHR;
    vulkanFunctions.vkBindImageMemory = vkBindImageMemory;
    vulkanFunctions.vkBindImageMemory2KHR = vkBindImageMemory2KHR;
    vulkanFunctions.vkCmdCopyBuffer = vkCmdCopyBuffer;
    vulkanFunctions.vkCreateImage = vkCreateImage;
    vulkanFunctions.vkDestroyImage = vkDestroyImage;
    vulkanFunctions.vkCreateBuffer = vkCreateBuffer;
    vulkanFunctions.vkDestroyBuffer = vkDestroyBuffer;
    vulkanFunctions.vkFlushMappedMemoryRanges = vkFlushMappedMemoryRanges;
    vulkanFunctions.vkFreeMemory = vkFreeMemory;
    vulkanFunctions.vkGetBufferMemoryRequirements = vkGetBufferMemoryRequirements;
    vulkanFunctions.vkGetBufferMemoryRequirements2KHR = vkGetBufferMemoryRequirements2KHR;
    vulkanFunctions.vkGetDeviceProcAddr = vkGetDeviceProcAddr;
    vulkanFunctions.vkGetImageMemoryRequirements = vkGetImageMemoryRequirements;
    vulkanFunctions.vkGetImageMemoryRequirements2KHR = vkGetImageMemoryRequirements2KHR;
    vulkanFunctions.vkGetDeviceProcAddr = vkGetDeviceProcAddr;
    vulkanFunctions.vkGetInstanceProcAddr = vkGetInstanceProcAddr;
    vulkanFunctions.vkGetPhysicalDeviceMemoryProperties = vkGetPhysicalDeviceMemoryProperties;
    vulkanFunctions.vkGetPhysicalDeviceMemoryProperties2KHR = vkGetPhysicalDeviceMemoryProperties2KHR;
    vulkanFunctions.vkGetPhysicalDeviceProperties = vkGetPhysicalDeviceProperties;
    vulkanFunctions.vkInvalidateMappedMemoryRanges = vkInvalidateMappedMemoryRanges;
    vulkanFunctions.vkMapMemory = vkMapMemory;
    vulkanFunctions.vkUnmapMemory = vkUnmapMemory;
    vulkanFunctions.vkGetImageMemoryRequirements2KHR = vkGetImageMemoryRequirements2KHR;

    VmaAllocatorCreateInfo allocatorInfo{};
    allocatorInfo.flags = 0;
    allocatorInfo.vulkanApiVersion = VK_API_VERSION_1_2;
    allocatorInfo.device = m_vkDevice;
    allocatorInfo.instance = m_vkInstance;
    allocatorInfo.physicalDevice = m_vkPhysicalDevice;
    allocatorInfo.pVulkanFunctions = &vulkanFunctions;

    result = vmaCreateAllocator(&allocatorInfo, &m_vmaAllocator);
    if (vkFAILED(result))
        return false;

    // -- Swapchain creation -- //
    m_nCurrentFrame = 0;
    CreateSwapChain(m_vkDevice, m_vkSwapchain, indices, window);

    // --  Image view creation -- //
    CreateImageViews();
    
    // -- Main render pass creation -- //
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = m_vkSwapchainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = FindDepthFormat();
    depthAttachment.format = FindDepthFormat();
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
    dependency.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = (uint32_t)attachments.size();
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    result = vkCreateRenderPass(m_vkDevice, &renderPassInfo, nullptr, &m_vkRenderPass);
    if (vkFAILED(result))
        return false;

    // -- Command Buffer creation -- //
    m_vkCommandBuffer.resize(MAX_FRAMES_IN_FLIGHT);

    MQueueFamilyIndices queueFamilyIndices = FindQueueFamilies(m_vkPhysicalDevice);

    VkCommandPoolCreateInfo transferPoolInfo{};
    transferPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    transferPoolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
    transferPoolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    result = vkCreateCommandPool(m_vkDevice, &transferPoolInfo, nullptr, &m_vkTransferCommandPool);
    if (vkFAILED(result))
        return false;

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    result = vkCreateCommandPool(m_vkDevice, &poolInfo, nullptr, &m_vkCommandPool);
    if (vkFAILED(result))
        return false;

    VkCommandBufferAllocateInfo allocateInfo{};
    allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocateInfo.commandBufferCount = (uint32_t)m_vkCommandBuffer.size();
    allocateInfo.commandPool = m_vkCommandPool;

    result = vkAllocateCommandBuffers(m_vkDevice, &allocateInfo, m_vkCommandBuffer.data());
    if (vkFAILED(result))
        return false;

    // -- Depth stencil creation -- //
    m_vkDepthStencilFormat = FindDepthFormat();

    VkImageCreateInfo depthImageInfo{};
    depthImageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    depthImageInfo.extent.width = m_vkSwapchainExtent.width;
    depthImageInfo.extent.height = m_vkSwapchainExtent.height;
    depthImageInfo.extent.depth = 1;
    depthImageInfo.arrayLayers = 1;
    depthImageInfo.mipLevels = 1;
    depthImageInfo.format = m_vkDepthStencilFormat;
    depthImageInfo.imageType = VK_IMAGE_TYPE_2D;
    depthImageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthImageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    depthImageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    depthImageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    depthImageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VmaAllocationCreateInfo depthAllocInfo{};
    depthAllocInfo.usage = VMA_MEMORY_USAGE_AUTO;

    result = vmaCreateImage(m_vmaAllocator, &depthImageInfo, &depthAllocInfo, &m_vkDepthStencilImage, &m_vmaDepthStencilAllocation, nullptr);
    if (vkFAILED(result))
        return false;

    VkImageViewCreateInfo depthViewInfo = {};
    depthViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    depthViewInfo.image = m_vkDepthStencilImage;
    depthViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    depthViewInfo.format = m_vkDepthStencilFormat;
    depthViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    depthViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    depthViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    depthViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    depthViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    depthViewInfo.subresourceRange.baseMipLevel = 0;
    depthViewInfo.subresourceRange.levelCount = 1;
    depthViewInfo.subresourceRange.baseArrayLayer = 0;
    depthViewInfo.subresourceRange.layerCount = 1;

    result = vkCreateImageView(m_vkDevice, &depthViewInfo, nullptr, &m_vkDepthStencilImageView);
    if (vkFAILED(result))
        return false;

    TransitionImageLayout(m_vkDepthStencilImage, m_vkDepthStencilFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL);

    // -- Framebuffer creation -- //
    CreateFrameBuffers();

    // -- Sync object creation -- //
    m_vkSwapChainBufferAvailable.resize(MAX_FRAMES_IN_FLIGHT);
    m_vkFinishedRendering.resize(MAX_FRAMES_IN_FLIGHT);
    m_vkInFlightFence.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        VkResult result1 = vkCreateSemaphore(m_vkDevice, &semaphoreInfo, nullptr, &m_vkSwapChainBufferAvailable[i]);
        VkResult result2 = vkCreateSemaphore(m_vkDevice, &semaphoreInfo, nullptr, &m_vkFinishedRendering[i]);
        VkResult result3 = vkCreateFence(m_vkDevice, &fenceInfo, nullptr, &m_vkInFlightFence[i]);

        if (vkFAILED(result))
            return false;
    }

    // -- Descriptor pool creation -- //
    std::array<VkDescriptorPoolSize, 2> poolSize{};
    poolSize[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize[0].descriptorCount = MAX_FRAMES_IN_FLIGHT * 4;
    poolSize[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSize[1].descriptorCount = MAX_FRAMES_IN_FLIGHT * 4;

    VkDescriptorPoolCreateInfo descriptorPoolInfo{};
    descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptorPoolInfo.poolSizeCount = (uint32_t)poolSize.size();
    descriptorPoolInfo.pPoolSizes = poolSize.data();
    descriptorPoolInfo.maxSets = MAX_FRAMES_IN_FLIGHT * 4;

    result = vkCreateDescriptorPool(m_vkDevice, &descriptorPoolInfo, nullptr, &m_vkDescriptorPool);
    if (vkFAILED(result))
        return false;

    return true;
}

void CVulkanRenderer::ShutdownRenderer()
{
    //vkDeviceWaitIdle(m_vkDevice); // Wait for logical device to finish before releasing everything

    vkDestroyDescriptorPool(m_vkDevice, m_vkDescriptorPool, nullptr);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        vkDestroySemaphore(m_vkDevice, m_vkSwapChainBufferAvailable[i], nullptr);
        vkDestroySemaphore(m_vkDevice, m_vkFinishedRendering[i], nullptr);
        vkDestroyFence(m_vkDevice, m_vkInFlightFence[i], nullptr);
    }

    vkDestroyCommandPool(m_vkDevice, m_vkCommandPool, nullptr);

    vkDestroyCommandPool(m_vkDevice, m_vkTransferCommandPool, nullptr);

    vkDestroyImageView(m_vkDevice, m_vkDepthStencilImageView, nullptr);

    vmaDestroyImage(m_vmaAllocator, m_vkDepthStencilImage, m_vmaDepthStencilAllocation);

    for (size_t i = 0; i < m_nNumSwapchainFrameBuffers; i++)
    {
        vkDestroyFramebuffer(m_vkDevice, m_pvkSwapchainFrameBuffers[i], nullptr);
    }

    for (size_t i = 0; i < m_nNumSwapchainImageViews; i++)
    {
        vkDestroyImageView(m_vkDevice, m_pvkSwapchainImageViews[i], nullptr);
    }

    vkDestroySwapchainKHR(m_vkDevice, m_vkSwapchain, nullptr);

    vkDestroySurfaceKHR(m_vkInstance, m_vkWindowSurface, nullptr);

    vkDestroyRenderPass(m_vkDevice, m_vkRenderPass, nullptr);

    vmaDestroyAllocator(m_vmaAllocator);

    vkDestroyDevice(m_vkDevice, nullptr);

#ifdef USE_VALIDATION_LAYERS
    DestroyDebugMessenger(m_vkInstance, m_vkDebugMessenger);
#endif

    vkDestroyInstance(m_vkInstance, nullptr);
}

IShader* CVulkanRenderer::CreateShader(char* data, size_t dataSize)
{
    CVulkanShader* pShader = new CVulkanShader(m_vkDevice, data, dataSize);
    return pShader;
}

IBuffer* CVulkanRenderer::CreateBuffer(char* data, size_t dataSize)
{
    CVulkanBuffer* pBuffer = new CVulkanBuffer(m_vkDevice, m_vmaAllocator, m_vkTransferCommandPool, m_vkGraphicsRenderQueue, data, dataSize);
    return pBuffer;
}

IImage* CVulkanRenderer::CreateImage(char* data, size_t dataSize, size_t width, size_t height)
{
    VmaAllocation stagingAllocation;
    VkBuffer stagingImage;

    VmaAllocation allocation;
    VkImage image;

    VkBufferCreateInfo stagingBufferInfo{};
    stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    stagingBufferInfo.size = dataSize;
    stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    stagingBufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VmaAllocationCreateInfo stagingAllocInfo{};
    stagingAllocInfo.usage = VMA_MEMORY_USAGE_AUTO;
    stagingAllocInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;

    vmaCreateBuffer(m_vmaAllocator, &stagingBufferInfo, &stagingAllocInfo, &stagingImage, &stagingAllocation, nullptr);

    void* pData;
    vmaMapMemory(m_vmaAllocator, stagingAllocation, &pData);
    memcpy(pData, data, dataSize);
    vmaUnmapMemory(m_vmaAllocator, stagingAllocation);

    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = static_cast<uint32_t>(width);
    imageInfo.extent.height = static_cast<uint32_t>(height);
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;

    VmaAllocationCreateInfo allocInfo{};
    allocInfo.usage = VMA_MEMORY_USAGE_AUTO;

    vmaCreateImage(m_vmaAllocator, &imageInfo, &allocInfo, &image, &allocation, nullptr);

    TransitionImageLayout(image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    CopyBufferToImage(stagingImage, image, (uint32_t)width, (uint32_t)height);
    TransitionImageLayout(image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    vmaDestroyBuffer(m_vmaAllocator, stagingImage, stagingAllocation);

    CVulkanImage* pImage = new CVulkanImage(m_vmaAllocator, image, allocation);

    return pImage;
}

IImageView* CVulkanRenderer::CreateImageView(IImage* image)
{
    CVulkanImage* pImage = dynamic_cast<CVulkanImage*>(image);
    CVulkanImageView* pView = new CVulkanImageView(m_vkDevice, pImage->GetImage());
    return pView;
}

IImageSampler* CVulkanRenderer::CreateImageSmpler(IImageView* view)
{
    CVulkanImageView* pView = dynamic_cast<CVulkanImageView*>(view);
    CVulkanImageSampler* pSampler = new CVulkanImageSampler(m_vkDevice, pView->GetImageView());
    return pSampler;
}

IConstantBuffer* CVulkanRenderer::CreateConstantBuffer(size_t dataSize, EUsageType usage)
{
    CVulkanConstantBuffer* pConstBuffer = new CVulkanConstantBuffer(m_vkDevice, m_vmaAllocator, dataSize);
    return pConstBuffer;
}

void CVulkanRenderer::UpdateConstantBuffer(IConstantBuffer* constantBuffer, void* data, size_t size)
{
    CVulkanConstantBuffer* pConstBuffer = dynamic_cast<CVulkanConstantBuffer*>(constantBuffer);
    pConstBuffer->UpdateBuffer(data, size, m_nCurrentFrame);
}

IDescriptorSet* CVulkanRenderer::CreateDescriptorSet(MDescriptorDesc* setDescs, size_t setDescsCount, MDescriptorSetLayoutDesc layoutDesc)
{
    std::vector<MVKDescriptorWriteSet> writeDesc;

    for (size_t i = 0; i < setDescsCount; i++)
    {
        for (size_t j = 0; j < MAX_FRAMES_IN_FLIGHT; j++)
        {
            MVKDescriptorWriteSet set = ConvertMDescriptorDescToVkDescriptorWrite(setDescs[i], j);;
            writeDesc.push_back(set);
        }
    }

    CVulkanDescriptorSet* pSet = new CVulkanDescriptorSet(m_vkDevice, m_vkDescriptorPool, writeDesc.data(), writeDesc.size(), layoutDesc);
    return pSet;
}

IPipelineObject* CVulkanRenderer::CreatePipeline(
    IShader* vertexShader, 
    IShader* fragmentShader, 
    IDescriptorSet** descriptorSets,
    size_t descriptorSetsCount)
{
    CVulkanShader* pvkVertShader = dynamic_cast<CVulkanShader*>(vertexShader);
    CVulkanShader* pvkFragShader = dynamic_cast<CVulkanShader*>(fragmentShader);

    std::vector<VkDescriptorSetLayout> setLayouts;
    for (size_t i = 0; i < descriptorSetsCount; i++)
    {
        CVulkanDescriptorSet* pDescriptorSet = dynamic_cast<CVulkanDescriptorSet*>(descriptorSets[i]);
        setLayouts.push_back(pDescriptorSet->GetLayout());
    }

    CVulkanPipelineObject* pPipeline = new CVulkanPipelineObject(
        m_vkDevice, 
        pvkVertShader->GetShader(), 
        pvkFragShader->GetShader(), 
        m_vkRenderPass, 
        setLayouts.data(), 
        setLayouts.size());

    return pPipeline;
}

void CVulkanRenderer::BeginCommandRecording()
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    vkBeginCommandBuffer(m_vkCommandBuffer[m_nCurrentFrame], &beginInfo);

    std::array<VkClearValue, 2> clearValues;
    clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
    clearValues[1].depthStencil = { 1.0f, 0 };

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.framebuffer = m_pvkSwapchainFrameBuffers[m_nImageIndex];
    renderPassInfo.renderArea.extent = m_vkSwapchainExtent;
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderPass = m_vkRenderPass;
    renderPassInfo.clearValueCount = (uint32_t)clearValues.size();
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(m_vkCommandBuffer[m_nCurrentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void CVulkanRenderer::EndCommandRecording()
{
    vkCmdEndRenderPass(m_vkCommandBuffer[m_nCurrentFrame]);
    VkResult result = vkEndCommandBuffer(m_vkCommandBuffer[m_nCurrentFrame]);
}

void CVulkanRenderer::SubmitCommandRecording()
{
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { m_vkSwapChainBufferAvailable[m_nCurrentFrame]};
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_vkCommandBuffer[m_nCurrentFrame];
    VkSemaphore signalSemaphores[] = { m_vkFinishedRendering[m_nCurrentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    vkQueueSubmit(m_vkGraphicsRenderQueue, 1, &submitInfo, m_vkInFlightFence[m_nCurrentFrame]);
}

void CVulkanRenderer::BindPipelineObject(IPipelineObject* pipeline)
{
    CVulkanPipelineObject* pPipeline = dynamic_cast<CVulkanPipelineObject*>(pipeline);
    m_pCurrentBoundPipeline = pPipeline;
    vkCmdBindPipeline(m_vkCommandBuffer[m_nCurrentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, pPipeline->GetPipeline());
}

void CVulkanRenderer::BindVertexBuffer(IBuffer* buffer, size_t offset)
{
    CVulkanBuffer* pBuffer = dynamic_cast<CVulkanBuffer*>(buffer);
    VkBuffer buffers[] = { pBuffer->GetBuffer() };
    VkDeviceSize offsets[] = { offset };
    vkCmdBindVertexBuffers(m_vkCommandBuffer[m_nCurrentFrame], 0, 1, buffers, offsets);
}

void CVulkanRenderer::BindIndexBuffer(IBuffer* buffer, size_t offset)
{
    CVulkanBuffer* pBuffer = dynamic_cast<CVulkanBuffer*>(buffer);
    vkCmdBindIndexBuffer(m_vkCommandBuffer[m_nCurrentFrame], pBuffer->GetBuffer(), offset, VK_INDEX_TYPE_UINT32);
}

void CVulkanRenderer::BindDescriptorSets(IDescriptorSet** descriptorSets, size_t descriptorSetsCount)
{
    std::vector<VkDescriptorSet> sets;
    for (size_t i = 0; i < descriptorSetsCount; i++)
    {
        CVulkanDescriptorSet* pSet = dynamic_cast<CVulkanDescriptorSet*>(descriptorSets[i]);
        sets.push_back(pSet->GetDescriptorSet(m_nCurrentFrame));
    }

    vkCmdBindDescriptorSets(
        m_vkCommandBuffer[m_nCurrentFrame],
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        m_pCurrentBoundPipeline->GetLayout(),
        0,
        (uint32_t)sets.size(),
        sets.data(),
        0,
        nullptr);
}

void CVulkanRenderer::SetViewportRect(MViewport viewport)
{
    VkViewport vkViewport;
    vkViewport.height = viewport.Height;
    vkViewport.maxDepth = viewport.MaxDepth;
    vkViewport.minDepth = viewport.MinDepth;
    vkViewport.width = viewport.Width;
    vkViewport.x = viewport.TopLeftX;
    vkViewport.y = viewport.TopLeftY;

    vkCmdSetViewport(m_vkCommandBuffer[m_nCurrentFrame], 0, 1, &vkViewport);
}

void CVulkanRenderer::SetScissorRect(MRect rect)
{
    VkRect2D vkRect;
    vkRect.offset = { 0, 0 };
    vkRect.extent = m_vkSwapchainExtent;
    vkCmdSetScissor(m_vkCommandBuffer[m_nCurrentFrame], 0, 1, &vkRect);
}

void CVulkanRenderer::DrawVertices(uint32_t numVerts)
{
    vkCmdDraw(m_vkCommandBuffer[m_nCurrentFrame], numVerts, 1, 0, 0);
}

void CVulkanRenderer::DrawIndices(uint32_t numInds)
{
    vkCmdDrawIndexed(m_vkCommandBuffer[m_nCurrentFrame], numInds, 1, 0, 0, 0);
}

//* Waits for the previous frame to be rendered
void CVulkanRenderer::WaitForPreviousFrame()
{
    vkWaitForFences(m_vkDevice, 1, &m_vkInFlightFence[m_nCurrentFrame], VK_TRUE, UINT64_MAX);
    vkResetFences(m_vkDevice, 1, &m_vkInFlightFence[m_nCurrentFrame]);
}

void CVulkanRenderer::WaitForDeviceToIdle()
{
    vkDeviceWaitIdle(m_vkDevice);
}

//* Aquires next image in the swap chain
void CVulkanRenderer::AquireNextSwapChainImage()
{
    vkAcquireNextImageKHR(m_vkDevice, m_vkSwapchain, UINT64_MAX, m_vkSwapChainBufferAvailable[m_nCurrentFrame], VK_NULL_HANDLE, &m_nImageIndex);
}

void CVulkanRenderer::Present()
{
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    VkSemaphore signalSemaphores[] = { m_vkFinishedRendering[m_nCurrentFrame]};
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores; // Wait for command buffer execution to finish before presenting
    VkSwapchainKHR swapChains[] = { m_vkSwapchain };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &m_vkSwapchain;
    presentInfo.pImageIndices = &m_nImageIndex;
    presentInfo.pResults = nullptr;
    
    VkResult result = vkQueuePresentKHR(m_vkPresentQueue, &presentInfo);
    m_nCurrentFrame = (m_nCurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

#ifdef USE_VALIDATION_LAYERS

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

#endif

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

VkPresentModeKHR CVulkanRenderer::ChooseSwapChainPresentMode(const std::vector<VkPresentModeKHR>& modes)
{
    return VK_PRESENT_MODE_IMMEDIATE_KHR; // YSYNC OFF
}

bool CVulkanRenderer::CreateSwapChain(VkDevice device, VkSwapchainKHR& swpachain, MQueueFamilyIndices indices, IWindow* pWindow)
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

    if (indices.graphicsFamily != indices.presentFamily) 
    {
        swapChainInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapChainInfo.queueFamilyIndexCount = 2;
        swapChainInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else 
    {
        swapChainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    swapChainInfo.preTransform = details.surfaceCapabilities.currentTransform;
    swapChainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapChainInfo.presentMode = mode;
    swapChainInfo.clipped = VK_TRUE;
    swapChainInfo.oldSwapchain = VK_NULL_HANDLE;

    VkResult result = vkCreateSwapchainKHR(device, &swapChainInfo, nullptr, &swpachain);
    if (vkFAILED(result))
        return false;

    uint32_t swapchainImageCount = 0;
    vkGetSwapchainImagesKHR(device, swpachain, &swapchainImageCount, nullptr);
    m_pvkSwapchainImages = new VkImage[swapchainImageCount];
    m_nNumSwapchainImages = swapchainImageCount;
    vkGetSwapchainImagesKHR(device, swpachain, &swapchainImageCount, m_pvkSwapchainImages);

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

VkShaderModule CVulkanRenderer::CreateShader(const std::vector<char>& buffer)
{
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = buffer.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(buffer.data());

    VkShaderModule shaderModule;
    if (vkFAILED(vkCreateShaderModule(m_vkDevice, &createInfo, nullptr, &shaderModule)))
    {
        return NULL;
    }

    return shaderModule;
}

VkFormat CVulkanRenderer::FindSupportedFormat(const std::vector<VkFormat>& canidates, VkImageTiling tiling, VkFormatFeatureFlags features)
{
    for (size_t i = 0; i < canidates.size(); i++)
    {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(m_vkPhysicalDevice, canidates[i], &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
        {
            return canidates[i];
        }
        else if(tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
        {
            return canidates[i];
        }
    }
    return canidates[0];
}

VkFormat CVulkanRenderer::FindDepthFormat()
{
    return FindSupportedFormat(
        { VK_FORMAT_D32_SFLOAT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
    ;
}

void CVulkanRenderer::TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = m_vkCommandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(m_vkDevice, &allocInfo, &commandBuffer); // Create a one time use command buffer

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    VkImageMemoryBarrier imageBarrier{};
    imageBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imageBarrier.oldLayout = oldLayout;
    imageBarrier.newLayout = newLayout;
    imageBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageBarrier.image = image;
    imageBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    imageBarrier.subresourceRange.baseMipLevel = 0;
    imageBarrier.subresourceRange.levelCount = 1;
    imageBarrier.subresourceRange.baseArrayLayer = 0;
    imageBarrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) 
    {
        imageBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

        if (HasStencilComponent(format)) 
        {
            imageBarrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
        }
    }
    else 
    {
        imageBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    }

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) 
    {
        imageBarrier.srcAccessMask = 0;
        imageBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) 
    {
        imageBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        imageBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
    {
        imageBarrier.srcAccessMask = 0;
        imageBarrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    }
    else 
    {
        return;
    }

    vkCmdPipelineBarrier(
        commandBuffer, 
        sourceStage, 
        destinationStage, 
        0, 
        0, 
        nullptr, 
        0, 
        nullptr, 
        1, 
        &imageBarrier
    );

    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(m_vkGraphicsRenderQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(m_vkGraphicsRenderQueue);

    vkFreeCommandBuffers(m_vkDevice, m_vkCommandPool, 1, &commandBuffer);
}

void CVulkanRenderer::CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = m_vkCommandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(m_vkDevice, &allocInfo, &commandBuffer); // Create a one time use command buffer

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = { 0, 0, 0 };
    region.imageExtent = {
        width,
        height,
        1
    };

    vkCmdCopyBufferToImage(
        commandBuffer,
        buffer,
        image,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1,
        &region
    );

    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(m_vkGraphicsRenderQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(m_vkGraphicsRenderQueue);

    vkFreeCommandBuffers(m_vkDevice, m_vkCommandPool, 1, &commandBuffer);
}

bool CVulkanRenderer::HasStencilComponent(VkFormat format)
{
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

MVKDescriptorWriteSet CVulkanRenderer::ConvertMDescriptorDescToVkDescriptorWrite(MDescriptorDesc desc, size_t constBufferIndex)
{
    MVKDescriptorWriteSet writeSet{};
    writeSet.descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeSet.descriptorWrite.dstSet = VK_NULL_HANDLE;
    writeSet.descriptorWrite.dstBinding = desc.binding;
    writeSet.descriptorWrite.dstArrayElement = 0;
    writeSet.descriptorWrite.descriptorCount = 1;

    switch (desc.type)
    {
    case UNIFORM_BUFFER:
        writeSet.descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        break;
    case IMAGE_SAMPLER:
        writeSet.descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        break;
    }

    // Translate MDescriptorBufferDesc to VkDescriptorBufferInfo if MDescriptorBufferDesc exists
    if (desc.pBufferInfo)
    {
        writeSet.bufferInfo.offset = (VkDeviceSize)desc.pBufferInfo->offset;
        writeSet.bufferInfo.range = (VkDeviceSize)desc.pBufferInfo->range;
        CVulkanConstantBuffer* pConstBuffer = dynamic_cast<CVulkanConstantBuffer*>(desc.pBufferInfo->pBuffer);
        writeSet.bufferInfo.buffer = pConstBuffer->GetBuffer(constBufferIndex);
        
        writeSet.descriptorWrite.pBufferInfo = &writeSet.bufferInfo;
    }

    // Tanslate MDescriptorImageDesc to VkDEscriptorImageInfo if MDescriptorImageDesc exists
    if (desc.pImageInfo)
    {
        writeSet.imageInfo.imageLayout = (VkImageLayout)desc.pImageInfo->imageLayout;
        CVulkanImageView* pView = dynamic_cast<CVulkanImageView*>(desc.pImageInfo->pImageView);
        CVulkanImageSampler* pSampler = dynamic_cast<CVulkanImageSampler*>(desc.pImageInfo->pImageSampler);
        writeSet.imageInfo.imageView = pView->GetImageView();
        writeSet.imageInfo.sampler = pSampler->GetSampler();

        writeSet.descriptorWrite.pImageInfo = &writeSet.imageInfo;
    }

    return writeSet;
}

void CVulkanRenderer::CreateImageViews()
{
    m_pvkSwapchainImageViews = new VkImageView[m_nNumSwapchainImages];
    m_nNumSwapchainImageViews = m_nNumSwapchainImages;

    for (size_t i = 0; i < m_nNumSwapchainImages; i++)
    {
        VkImageViewCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = m_pvkSwapchainImages[i];
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

        VkResult result = vkCreateImageView(m_vkDevice, &createInfo, nullptr, &m_pvkSwapchainImageViews[i]);
    }
}

bool CVulkanRenderer::CreateFrameBuffers()
{
    m_pvkSwapchainFrameBuffers = new VkFramebuffer[FRAME_BUFFER_COUNT];
    m_nNumSwapchainFrameBuffers = FRAME_BUFFER_COUNT;

    VkResult result;

    for (size_t i = 0; i < m_nNumSwapchainImageViews; i++) 
    {
        std::array<VkImageView, 2> attachments = {
            m_pvkSwapchainImageViews[i],
            m_vkDepthStencilImageView
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = m_vkRenderPass;
        framebufferInfo.attachmentCount = (uint32_t)attachments.size();
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = m_vkSwapchainExtent.width;
        framebufferInfo.height = m_vkSwapchainExtent.height;
        framebufferInfo.layers = 1;

        result = vkCreateFramebuffer(m_vkDevice, &framebufferInfo, nullptr, &m_pvkSwapchainFrameBuffers[i]);

        if (vkFAILED(result))
            return false;
    }
    return true;
}

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
    char buffer[2048];
    sprintf_s(buffer, "%s\n", pCallbackData->pMessage);
    OutputDebugStringA(buffer);

    return VK_FALSE;
}

void CreateVulkanRenderer(IRenderer** ppRenderer)
{
    *ppRenderer = new CVulkanRenderer();
}

CVulkanShader::CVulkanShader(VkDevice device, char* data, size_t dataSize)
{
    m_vkDeviceRef = device;

    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = dataSize;
    createInfo.pCode = reinterpret_cast<const uint32_t*>(data);

    if (vkFAILED(vkCreateShaderModule(device, &createInfo, nullptr, &m_vkShader)))
    {
        m_vkShader = VK_NULL_HANDLE;
    }
}

CVulkanShader::~CVulkanShader()
{
    vkDestroyShaderModule(m_vkDeviceRef, m_vkShader, nullptr);
}

void CVulkanShader::ReleaseShader()
{
    delete this;
}

CVulkanPipelineObject::CVulkanPipelineObject(
    VkDevice device, 
    VkShaderModule vertShader, 
    VkShaderModule fragShader, 
    VkRenderPass renderPass, 
    VkDescriptorSetLayout* setLayouts, 
    size_t setLayoutCounts)
{
    m_vkDeviceRef = device;

    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShader;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShader;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

    VkDynamicState dynamicStates[] = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(2);
    dynamicState.pDynamicStates = dynamicStates;

    VkVertexInputBindingDescription bindingDesc{};
    bindingDesc.binding = 0;
    bindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    bindingDesc.stride = sizeof(MGenericVertex);

    VkVertexInputAttributeDescription vertexAttributeDesc[3];
    vertexAttributeDesc[0].binding = 0;
    vertexAttributeDesc[0].location = 0;
    vertexAttributeDesc[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    vertexAttributeDesc[0].offset = offsetof(MGenericVertex, pos);

    vertexAttributeDesc[1].binding = 0;
    vertexAttributeDesc[1].location = 1;
    vertexAttributeDesc[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    vertexAttributeDesc[1].offset = offsetof(MGenericVertex, norm);

    vertexAttributeDesc[2].binding = 0;
    vertexAttributeDesc[2].location = 2;
    vertexAttributeDesc[2].format = VK_FORMAT_R32G32_SFLOAT;
    vertexAttributeDesc[2].offset = offsetof(MGenericVertex, tcoords);


    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions = &bindingDesc;
    vertexInputInfo.vertexAttributeDescriptionCount = 3;
    vertexInputInfo.pVertexAttributeDescriptions = vertexAttributeDesc;

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f; // Optional
    rasterizer.depthBiasClamp = 0.0f; // Optional
    rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f; // Optional
    multisampling.pSampleMask = nullptr; // Optional
    multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
    multisampling.alphaToOneEnable = VK_FALSE; // Optional

    VkPipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.stencilTestEnable = VK_FALSE;
    depthStencil.minDepthBounds = 0.0f;
    depthStencil.maxDepthBounds = 1.0f;

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional
    colorBlendAttachment.blendEnable = VK_TRUE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f; // Optional
    colorBlending.blendConstants[1] = 0.0f; // Optional
    colorBlending.blendConstants[2] = 0.0f; // Optional
    colorBlending.blendConstants[3] = 0.0f; // Optional

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = (uint32_t)setLayoutCounts;
    pipelineLayoutInfo.pSetLayouts = setLayouts;

    VkResult result = vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &m_vkPipelineLayout);

    if (vkFAILED(result))
        m_vkPipelineLayout = VK_NULL_HANDLE;

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = m_vkPipelineLayout;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
    pipelineInfo.basePipelineIndex = -1; // Optional

    result = vkCreateGraphicsPipelines(m_vkDeviceRef, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_vkPipeline);

    if (vkFAILED(result))
        m_vkPipeline = VK_NULL_HANDLE;
}

CVulkanPipelineObject::~CVulkanPipelineObject()
{
    vkDestroyPipelineLayout(m_vkDeviceRef, m_vkPipelineLayout, nullptr);
    vkDestroyPipeline(m_vkDeviceRef, m_vkPipeline, nullptr);
}

void CVulkanPipelineObject::Release()
{
    delete this;
}

CVulkanBuffer::CVulkanBuffer(VkDevice device, VmaAllocator allocator, VkCommandPool commandPool, VkQueue queue, void* data, size_t dataSize)
    :m_vkBuffer(VK_NULL_HANDLE), m_vmaAllocation(VK_NULL_HANDLE), m_vmaAllocatorRef(VK_NULL_HANDLE), m_vkDeviceRef(VK_NULL_HANDLE)
{
    m_vmaAllocatorRef = allocator;
    m_vkDeviceRef = device;

    // -- Buffer Creation -- //
    VkBufferCreateInfo stagingBufferInfo{};
    stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    stagingBufferInfo.size = dataSize;
    stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    stagingBufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VmaAllocationCreateInfo stagingAllocInfo{};
    stagingAllocInfo.usage = VMA_MEMORY_USAGE_AUTO;
    stagingAllocInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;;

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = dataSize;
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VmaAllocationCreateInfo allocInfo = {};
    allocInfo.usage = VMA_MEMORY_USAGE_AUTO;
    allocInfo.pool = VK_NULL_HANDLE;

    VkBuffer stagingBuffer;
    VmaAllocation stagingAllocation;

    VkBuffer buffer;
    VmaAllocation bufferAllocation;

    vmaCreateBuffer(allocator, &stagingBufferInfo, &stagingAllocInfo, &stagingBuffer, &stagingAllocation, nullptr);
    vmaCreateBuffer(allocator, &bufferInfo, &allocInfo, &buffer, &bufferAllocation, nullptr);

    // -- Buffer mapping -- //
    void* mappedData;
    vmaMapMemory(m_vmaAllocatorRef, stagingAllocation, &mappedData);
    memcpy(mappedData, data, dataSize);
    vmaUnmapMemory(m_vmaAllocatorRef, stagingAllocation);
    vmaFlushAllocation(m_vmaAllocatorRef, stagingAllocation, 0, dataSize);

    // -- Buffer Transfer -- //
    VkCommandBufferAllocateInfo cmdInfo{};
    cmdInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmdInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cmdInfo.commandPool = commandPool;
    cmdInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(m_vkDeviceRef, &cmdInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    VkBufferCopy copyRegion{};
    copyRegion.size = dataSize;
    vkCmdCopyBuffer(commandBuffer, stagingBuffer, buffer, 1, &copyRegion);

    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(queue);

    vkFreeCommandBuffers(m_vkDeviceRef, commandPool, 1, &commandBuffer);

    vmaDestroyBuffer(m_vmaAllocatorRef, stagingBuffer, stagingAllocation);

    m_vkBuffer = buffer;
    m_vmaAllocation = bufferAllocation;
}

CVulkanBuffer::~CVulkanBuffer()
{
    vmaDestroyBuffer(m_vmaAllocatorRef, m_vkBuffer, m_vmaAllocation);
}

void CVulkanBuffer::ReleaseBuffer()
{
    delete this;
}

CVulkanConstantBuffer::CVulkanConstantBuffer(VkDevice device, VmaAllocator allocator, size_t bufferSize)
    :m_vkDeviceRef(VK_NULL_HANDLE), m_vmaAllocatorRef(VK_NULL_HANDLE)
{
    m_vkDeviceRef = device;
    m_vmaAllocatorRef = allocator;
    
    // -- uniform buffer creation -- //
    m_vkUniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    m_vmaUniformAllocations.resize(MAX_FRAMES_IN_FLIGHT);
    m_pUniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);
    
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = bufferSize;
    bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    
    VmaAllocationCreateInfo allocInfo{};
    allocInfo.usage = VMA_MEMORY_USAGE_AUTO;
    allocInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        vmaCreateBuffer(allocator, &bufferInfo, &allocInfo, &m_vkUniformBuffers[i], &m_vmaUniformAllocations[i], nullptr);
        vmaMapMemory(m_vmaAllocatorRef, m_vmaUniformAllocations[i], &m_pUniformBuffersMapped[i]);
    }
}

CVulkanConstantBuffer::~CVulkanConstantBuffer()
{
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        vmaUnmapMemory(m_vmaAllocatorRef, m_vmaUniformAllocations[i]);
        vmaDestroyBuffer(m_vmaAllocatorRef, m_vkUniformBuffers[i], m_vmaUniformAllocations[i]);
    }
}

void CVulkanConstantBuffer::UpdateBuffer(void* data, size_t size, size_t index)
{
    memcpy(m_pUniformBuffersMapped[index], data, size);
}

void CVulkanConstantBuffer::ReleaseBuffer()
{
    delete this;
}

CVulkanImage::CVulkanImage(VmaAllocator allocator, VkImage image, VmaAllocation allocation)
    :m_vkImage(image), m_vmaAllocation(allocation), m_vmaAllocatorRef(allocator)
{
}

CVulkanImage::~CVulkanImage()
{
    vmaDestroyImage(m_vmaAllocatorRef, m_vkImage, m_vmaAllocation);
}

void CVulkanImage::ReleaseImage()
{
    delete this;
}

CVulkanImageView::CVulkanImageView(VkDevice device, VkImage image)
{
    m_vkDeviceRef = device;

    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    vkCreateImageView(device, &viewInfo, nullptr, &m_vkImageView);
}

CVulkanImageView::~CVulkanImageView()
{
    vkDestroyImageView(m_vkDeviceRef, m_vkImageView, nullptr);
}

void CVulkanImageView::ReleaseImageView()
{
    delete this;
}

CVulkanImageSampler::CVulkanImageSampler(VkDevice device, VkImageView view)
    :m_vkDeviceRef(device), m_vkSampler(VK_NULL_HANDLE)
{
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_TRUE;
    samplerInfo.maxAnisotropy = 8;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 0.0f;

    vkCreateSampler(device, &samplerInfo, nullptr, &m_vkSampler);
}

CVulkanImageSampler::~CVulkanImageSampler()
{
    vkDestroySampler(m_vkDeviceRef, m_vkSampler, nullptr);
}

void CVulkanImageSampler::ReleaseSampler()
{
    delete this;
}

CVulkanDescriptorSet::CVulkanDescriptorSet(VkDevice device, VkDescriptorPool pool, MVKDescriptorWriteSet* setDescs, size_t setDescsCount, MDescriptorSetLayoutDesc layoutDesc)
    :m_vkDeviceRef(device), m_vkDescriptorLayout(VK_NULL_HANDLE), m_vkDescriptorSets()
{
    // -- Translate MDescriptorSetLayoutBindingDesc to VkDescriptorSetLayoutBinding -- //
    std::vector<VkDescriptorSetLayoutBinding> bindings;

    for (uint32_t i = 0; i < layoutDesc.bindingCount; i++)
    {
        VkDescriptorSetLayoutBinding vkBinding{};
        vkBinding.binding = layoutDesc.pBindings[i].binding;
        vkBinding.descriptorCount = layoutDesc.pBindings[i].descriptorCount;
        vkBinding.stageFlags = (VkShaderStageFlags)layoutDesc.pBindings[i].stageFlags;
        vkBinding.descriptorType = (VkDescriptorType)layoutDesc.pBindings[i].type;
        bindings.push_back(vkBinding);
    }
    
    // -- Translate MDescriptorSetLayoutDesc to VkDescriptorSetLayoutCreateInfo -- //
    VkDescriptorSetLayoutCreateInfo layoutCreateInfo{};
    layoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutCreateInfo.bindingCount = layoutDesc.bindingCount;
    layoutCreateInfo.pBindings = bindings.data();

    vkCreateDescriptorSetLayout(device, &layoutCreateInfo, nullptr, &m_vkDescriptorLayout);

    // -- Allocating descriptors -- //
    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, m_vkDescriptorLayout);

    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = pool;
    allocInfo.descriptorSetCount = MAX_FRAMES_IN_FLIGHT;
    allocInfo.pSetLayouts = layouts.data();

    m_vkDescriptorSets.resize(MAX_FRAMES_IN_FLIGHT);

    vkAllocateDescriptorSets(device, &allocInfo, m_vkDescriptorSets.data());

    // -- Writing in decriptors -- //
    for (size_t i = 0; i < setDescsCount;)
    {
        for (size_t j = 0; j < MAX_FRAMES_IN_FLIGHT; j++, i++)
        {
            setDescs[i].descriptorWrite.dstSet = m_vkDescriptorSets[j];
            setDescs[i].descriptorWrite.pBufferInfo = &setDescs[i].bufferInfo;
            setDescs[i].descriptorWrite.pImageInfo = &setDescs[i].imageInfo;
            vkUpdateDescriptorSets(device, 1, &setDescs[i].descriptorWrite, 0, nullptr);
        }
    }
}

CVulkanDescriptorSet::~CVulkanDescriptorSet()
{
    vkDestroyDescriptorSetLayout(m_vkDeviceRef, m_vkDescriptorLayout, nullptr);
}

void CVulkanDescriptorSet::ReleaseDescriptorSet()
{
    delete this;
}
