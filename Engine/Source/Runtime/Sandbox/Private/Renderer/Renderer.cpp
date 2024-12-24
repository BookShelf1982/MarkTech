#include "Renderer.h"
#include "VulkanHelper.h"

#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace MarkTech
{
	bool CreateRendererSwapchain(
		VkDevice device,
		VkPhysicalDevice physicalDevice,
		VkSurfaceKHR surface,
		const RendererConfig& config,
		Swapchain& swapchain
	)
	{
		VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
		if (!config.enableVSync)
			presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;

		VkFormat swapchainFormat;

		if (CreateVulkanSwapchain(
			device,
			physicalDevice,
			surface,
			presentMode,
			&swapchain.extent,
			&swapchainFormat,
			&swapchain.swapchain
		) != VK_SUCCESS)
		{
			return false;
		}

		if (CreateVulkanSwapchainRenderPass(device, swapchainFormat, &swapchain.renderPass) != VK_SUCCESS)
			return false;

		if (CreateVulkanSwapchainFramebuffers(
			device,
			swapchain.swapchain,
			swapchain.renderPass,
			swapchainFormat,
			swapchain.extent,
			&swapchain.framebufferCount,
			&swapchain.pFramebuffers,
			&swapchain.pImageViews
		) != VK_SUCCESS)
		{
			return false;
		}

		return true;
	}

	VkResult CreateSpritePipeline(
		VkDevice device,
		VkRenderPass renderPass,
		VkShaderModule vertexShader,
		VkShaderModule fragmentShader,
		VkPipeline* pPipeline,
		VkPipelineLayout* pPipelineLayout,
		VkDescriptorSetLayout* pSetLayout
	)
	{
		VkResult result;
		VkDescriptorSetLayoutBinding bindings[3] = {};
		bindings[0].binding = 0;
		bindings[0].descriptorCount = 1;
		bindings[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;

		bindings[1].binding = 1;
		bindings[1].descriptorCount = 1;
		bindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;

		bindings[2].binding = 2;
		bindings[2].descriptorCount = 1;
		bindings[2].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		bindings[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

		VkDescriptorSetLayoutCreateInfo setLayoutInfo = {};
		setLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		setLayoutInfo.bindingCount = 3;
		setLayoutInfo.pBindings = bindings;

		result = vkCreateDescriptorSetLayout(device, &setLayoutInfo, nullptr, pSetLayout);
		if (result != VK_SUCCESS)
			return result;

		VkPipelineLayoutCreateInfo layoutInfo = {};
		layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		layoutInfo.flags = 0;
		layoutInfo.pSetLayouts = pSetLayout;
		layoutInfo.setLayoutCount = 1;

		result = vkCreatePipelineLayout(device, &layoutInfo, nullptr, pPipelineLayout);
		if (result != VK_SUCCESS)
			return result;

		VkPipelineShaderStageCreateInfo shaderInfos[2] = {};
		shaderInfos[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderInfos[0].module = vertexShader;
		shaderInfos[0].pName = "main";
		shaderInfos[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		
		shaderInfos[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderInfos[1].module = fragmentShader;
		shaderInfos[1].pName = "main";
		shaderInfos[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;

		VkPipelineVertexInputStateCreateInfo vertexInput = {};
		vertexInput.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

		VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		VkPipelineViewportStateCreateInfo viewportState = {};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.scissorCount = 1;
		viewportState.viewportCount = 1;

		VkPipelineMultisampleStateCreateInfo multisampleInfo = {};
		multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		VkPipelineRasterizationStateCreateInfo rasterInfo = {};
		rasterInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterInfo.polygonMode = VK_POLYGON_MODE_FILL;
		rasterInfo.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterInfo.lineWidth = 1.0f;

		VkPipelineColorBlendAttachmentState attachment = {};
		attachment.colorWriteMask = VK_COLOR_COMPONENT_A_BIT | VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT;

		VkPipelineColorBlendStateCreateInfo colorBlendInfo = {};
		colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlendInfo.pAttachments = &attachment;
		colorBlendInfo.attachmentCount = 1;

		VkDynamicState dynamicStates[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

		VkPipelineDynamicStateCreateInfo dynamicState = {};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = 2;
		dynamicState.pDynamicStates = dynamicStates;

		VkGraphicsPipelineCreateInfo pipelineInfo = {};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.pNext = nullptr;
		pipelineInfo.flags = 0;
		pipelineInfo.pStages = shaderInfos;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pVertexInputState = &vertexInput;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pRasterizationState = &rasterInfo;
		pipelineInfo.pMultisampleState = &multisampleInfo;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pColorBlendState = &colorBlendInfo;
		pipelineInfo.pDynamicState = &dynamicState;
		pipelineInfo.layout = *pPipelineLayout;
		pipelineInfo.renderPass = renderPass;
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineInfo.basePipelineIndex = -1;

		result = vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, pPipeline);
		return result;
	}

	void GetMemoryTypeIndices(VkPhysicalDevice physicalDevice, MemoryTypeIndicies& indices)
	{
		VkPhysicalDeviceMemoryProperties memoryProps;
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProps);

		U32 indicesFound = 0;

		for (U32 i = 0; i < memoryProps.memoryTypeCount; i++)
		{
			if (indicesFound == 2)
				return;

			if (memoryProps.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
			{
				indices.deviceLocalMemory = i;
				indicesFound++;
				continue;
			}

			if (memoryProps.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
			{
				indices.hostVisibleMemory = i;
				indicesFound++;
				continue;
			}
		}
	}

	void ReadShaderFile(const char* pFilename, U32** ppData, U64* size)
	{
		FILE* file;
		fopen_s(&file, pFilename, "rb");
		if (!file)
			return;

		fseek(file, 0, SEEK_END);
		*size = ftell(file);
		*ppData = (U32*)malloc(*size);
		fseek(file, 0, SEEK_SET);
		fread(*ppData, *size, 1, file);
		fclose(file);
	}

	VkResult CreateSpriteImage(VkDevice device, VkFormat format, VkExtent2D extent, VkImage* pImage)
	{
		VkImageCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		info.imageType = VK_IMAGE_TYPE_2D;
		info.format = format;
		info.extent.width = extent.width;
		info.extent.height = extent.height;
		info.extent.depth = 1;
		info.mipLevels = 1;
		info.arrayLayers = 1;
		info.samples = VK_SAMPLE_COUNT_1_BIT;
		info.tiling = VK_IMAGE_TILING_OPTIMAL;
		info.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		return vkCreateImage(device, &info, nullptr, pImage);
	}

	bool InitRenderer(const RendererConfig& config, Renderer& renderer)
	{
		if (CreateVulkanInstance("Pac-Man", VK_MAKE_VERSION(0, 0, 1), &renderer.instance) != VK_SUCCESS)
			return false;

#ifdef DEBUG
		CreateDebugMessenger(renderer.instance, &renderer.debugMessenger);
#endif

#ifdef MT_PLATFORM_WINDOWS
		if (CreateWin32Surface(renderer.instance, config.pWindow->hWnd, &renderer.windowSurface) != VK_SUCCESS)
			return false;
#endif
		VkPhysicalDevice physicalDevice = GetSuitableDevice(renderer.instance, renderer.windowSurface, &renderer.graphicsQueueIndex);

		if (CreateVulkanLogicalDevice(physicalDevice, renderer.graphicsQueueIndex, &renderer.device) != VK_SUCCESS)
			return false;

		vkGetDeviceQueue(renderer.device, renderer.graphicsQueueIndex, 0, &renderer.graphicsQueue);

		if (!CreateRendererSwapchain(
			renderer.device,
			physicalDevice,
			renderer.windowSurface,
			config,
			renderer.swapchain)
			)
		{
			return false;
		}

		GetMemoryTypeIndices(physicalDevice, renderer.memoryTypes);

		VkCommandPoolCreateInfo commandPoolInfo;
		commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolInfo.pNext = nullptr;
		commandPoolInfo.flags = 0;
		commandPoolInfo.queueFamilyIndex = renderer.graphicsQueueIndex;
		
		if (vkCreateCommandPool(renderer.device, &commandPoolInfo, nullptr, &renderer.commandPool) != VK_SUCCESS)
			return false;

		CreateVulkanFence(renderer.device, 0, &renderer.finishedRendering);
		CreateVulkanSemaphore(renderer.device, &renderer.acquiredNextImage);

		U32* pVertData = nullptr;
		U64 vertDataSize = 0;

		U32* pFragData = nullptr;
		U64 fragDataSize = 0;

		ReadShaderFile("vert.spv", &pVertData, &vertDataSize);
		ReadShaderFile("frag.spv", &pFragData, &fragDataSize);

		CreateVulkanShader(renderer.device, pVertData, vertDataSize, &renderer.vertexShader);
		CreateVulkanShader(renderer.device, pFragData, fragDataSize, &renderer.fragmentShader);

		free(pVertData);
		free(pFragData);

		if (CreateSpritePipeline(
			renderer.device,
			renderer.swapchain.renderPass,
			renderer.vertexShader,
			renderer.fragmentShader,
			&renderer.spritePipeline.pipeline,
			&renderer.spritePipeline.pipelineLayout,
			&renderer.spritePipeline.setLayout
		) != VK_SUCCESS)
		{
			return false;
		}

		VkDescriptorPoolSize descriptorSizes[3] = {};
		descriptorSizes[0].descriptorCount = 8;
		descriptorSizes[0].type = VK_DESCRIPTOR_TYPE_SAMPLER;
		descriptorSizes[1].descriptorCount = 8;
		descriptorSizes[1].type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
		descriptorSizes[2].descriptorCount = 8;
		descriptorSizes[2].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

		if (CreateVulkanDescriptorPool(renderer.device, 8, descriptorSizes, 3, &renderer.descriptorPool))
			return false;

		VkSamplerCreateInfo samplerInfo = {};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.anisotropyEnable = VK_FALSE;
		samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
		samplerInfo.mipLodBias = 1.0f;
		samplerInfo.minLod = 0;
		samplerInfo.maxLod = 0;
		switch (config.textureFiltering)
		{
		case TEXTURE_FILTERING_NONE:
			samplerInfo.magFilter = VK_FILTER_NEAREST;
			samplerInfo.minFilter = VK_FILTER_NEAREST;
			samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
			break;
		case TEXTURE_FILTERING_BILINEAR:
			samplerInfo.magFilter = VK_FILTER_LINEAR;
			samplerInfo.minFilter = VK_FILTER_LINEAR;
			samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
			break;
		}

		vkCreateSampler(renderer.device, &samplerInfo, nullptr, &renderer.imageSampler);

		renderer.textureMemory = CreateDeviceStackAllocator(renderer.device, MEBIBYTE * 256, renderer.memoryTypes.deviceLocalMemory);
		renderer.hostMemory = CreateDeviceStackAllocator(renderer.device, MEBIBYTE * 128, renderer.memoryTypes.hostVisibleMemory);

		I32 width, height, channels = 0;
		U8* pImgData = stbi_load("pebbles.png", &width, &height, &channels, 4);
		U64 size = 4 * width * height;

		VkBuffer imageStagingBuffer;
		CreateVulkanBuffer(renderer.device, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, &imageStagingBuffer);
		CreateSpriteImage(renderer.device, VK_FORMAT_R8G8B8A8_SRGB, { (U32)width, (U32)height }, &renderer.spriteImage);
		BindImageToDeviceStack(renderer.device, renderer.spriteImage, renderer.textureMemory);
		CreateVulkanImageView(renderer.device, renderer.spriteImage, VK_FORMAT_R8G8B8A8_SRGB, { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 }, &renderer.spriteImageView);
		BindBufferToDeviceStack(renderer.device, imageStagingBuffer, renderer.hostMemory);

		Matrix4x4 transformMatrix = Multiply4x4(Traslation4x4(0, 0, 0), Scale4x4(0.25, 0.25, 0.25));


		void* ptr;
		vkMapMemory(renderer.device, renderer.hostMemory.memory, 0, renderer.hostMemory.currentOffset, 0, &ptr);
		memcpy(ptr, pImgData, size);
		vkUnmapMemory(renderer.device, renderer.hostMemory.memory);

		stbi_image_free(pImgData);

		VkBuffer uniformStagingBuffer;
		CreateVulkanBuffer(renderer.device, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, &uniformStagingBuffer);
		U64 startingOffset = renderer.hostMemory.currentOffset;
		BindBufferToDeviceStack(renderer.device, uniformStagingBuffer, renderer.hostMemory);

		CreateVulkanBuffer(renderer.device, sizeof(Matrix4x4), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, &renderer.uniformBuffer);
		BindBufferToDeviceStack(renderer.device, renderer.uniformBuffer, renderer.textureMemory);

		vkMapMemory(renderer.device, renderer.hostMemory.memory, startingOffset, sizeof(Matrix4x4), 0, &ptr);
		memcpy(ptr, &transformMatrix, sizeof(Matrix4x4));
		vkUnmapMemory(renderer.device, renderer.hostMemory.memory);

		VkCommandBuffer commandBuffer;
		AllocateVulkanCommandBuffer(renderer.device, renderer.commandPool, &commandBuffer);
		BeginVulkanCommandBuffer(commandBuffer);
		
		VkImageMemoryBarrier imageBarrier;
		imageBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		imageBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		imageBarrier.dstQueueFamilyIndex = renderer.graphicsQueueIndex;
		imageBarrier.image = renderer.spriteImage;
		imageBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		imageBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageBarrier.pNext = nullptr;
		imageBarrier.srcAccessMask = VK_ACCESS_NONE;
		imageBarrier.srcQueueFamilyIndex = renderer.graphicsQueueIndex;
		imageBarrier.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
		VulkanImageMamoryBarrier(commandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, &imageBarrier);

		VkBufferImageCopy bufferImageCopy;
		bufferImageCopy.bufferOffset = 0;
		bufferImageCopy.bufferImageHeight = height;
		bufferImageCopy.bufferRowLength = width;
		bufferImageCopy.imageExtent = { (U32)width, (U32)height, 1 };
		bufferImageCopy.imageOffset = { 0, 0, 0 };
		bufferImageCopy.imageSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1 };
		vkCmdCopyBufferToImage(commandBuffer, imageStagingBuffer, renderer.spriteImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &bufferImageCopy);

		VkBufferCopy bufferCopy;
		bufferCopy.srcOffset = 0;
		bufferCopy.size = sizeof(Matrix4x4);
		bufferCopy.dstOffset = 0;
		vkCmdCopyBuffer(commandBuffer, uniformStagingBuffer, renderer.uniformBuffer, 1, &bufferCopy);

		imageBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		imageBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		imageBarrier.dstQueueFamilyIndex = renderer.graphicsQueueIndex;
		imageBarrier.image = renderer.spriteImage;
		imageBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		imageBarrier.pNext = nullptr;
		imageBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		imageBarrier.srcQueueFamilyIndex = renderer.graphicsQueueIndex;
		imageBarrier.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
		VulkanImageMamoryBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT, &imageBarrier);
		vkEndCommandBuffer(commandBuffer);

		SumbitToVulkanQueue(renderer.graphicsQueue, commandBuffer, nullptr, renderer.finishedRendering);
		vkWaitForFences(renderer.device, 1, &renderer.finishedRendering, VK_TRUE, UINT64_MAX);
		vkResetFences(renderer.device, 1, &renderer.finishedRendering);

		vkFreeCommandBuffers(renderer.device, renderer.commandPool, 1, &commandBuffer);
		vkDestroyBuffer(renderer.device, uniformStagingBuffer, nullptr);
		vkDestroyBuffer(renderer.device, imageStagingBuffer, nullptr);

		AllocateVulkanDescriptorSet(renderer.device, renderer.descriptorPool, renderer.spritePipeline.setLayout, &renderer.descriptorSet);

		VkDescriptorImageInfo descriptorSamplerInfo;
		descriptorSamplerInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		descriptorSamplerInfo.imageView = VK_NULL_HANDLE;
		descriptorSamplerInfo.sampler = renderer.imageSampler;

		VkDescriptorImageInfo descriptorImageInfo;
		descriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		descriptorImageInfo.imageView = renderer.spriteImageView;
		descriptorImageInfo.sampler = VK_NULL_HANDLE;

		VkDescriptorBufferInfo descriptorUniformInfo;
		descriptorUniformInfo.buffer = renderer.uniformBuffer;
		descriptorUniformInfo.offset = 0;
		descriptorUniformInfo.range = sizeof(Matrix4x4);

		VkWriteDescriptorSet write[3] = {};
		write[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write[0].dstSet = renderer.descriptorSet;
		write[0].dstBinding = 1;
		write[0].dstArrayElement = 0;
		write[0].descriptorCount = 1;
		write[0].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
		write[0].pImageInfo = &descriptorSamplerInfo;

		write[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write[1].dstSet = renderer.descriptorSet;
		write[1].dstBinding = 0;
		write[1].dstArrayElement = 0;
		write[1].descriptorCount = 1;
		write[1].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
		write[1].pImageInfo = &descriptorImageInfo;

		write[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write[2].dstSet = renderer.descriptorSet;
		write[2].dstBinding = 2;
		write[2].dstArrayElement = 0;
		write[2].descriptorCount = 1;
		write[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		write[2].pBufferInfo = &descriptorUniformInfo;
		vkUpdateDescriptorSets(renderer.device, 3, write, 0, nullptr);

		return true;
	}

	void RenderFrame(Renderer& renderer)
	{
		VkCommandBuffer cmdBuffer;
		AllocateVulkanCommandBuffer(renderer.device, renderer.commandPool, &cmdBuffer);
		BeginVulkanCommandBuffer(cmdBuffer);

		U32 imageIndex = 0;
		vkAcquireNextImageKHR(renderer.device, renderer.swapchain.swapchain, UINT64_MAX, renderer.acquiredNextImage, VK_NULL_HANDLE, &imageIndex);

		BeginVulkanRenderPass(cmdBuffer, renderer.swapchain.renderPass, renderer.swapchain.pFramebuffers[imageIndex], renderer.swapchain.extent);

		VkViewport viewport;
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.height = (float)renderer.swapchain.extent.height;
		viewport.width = (float)renderer.swapchain.extent.width;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(cmdBuffer, 0, 1, &viewport);

		VkRect2D scissorRect;
		scissorRect.extent = renderer.swapchain.extent;
		scissorRect.offset = { 0, 0 };
		vkCmdSetScissor(cmdBuffer, 0, 1, &scissorRect);

		vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, renderer.spritePipeline.pipeline);
		vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, renderer.spritePipeline.pipelineLayout, 0, 1, &renderer.descriptorSet, 0, nullptr);
		vkCmdDraw(cmdBuffer, 6, 1, 0, 0);

		vkCmdEndRenderPass(cmdBuffer);
		vkEndCommandBuffer(cmdBuffer);

		VulkanQueueSubmitSyncObjects syncObjects = {};
		VkPipelineStageFlags waitStageDst = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		syncObjects.pWaitDstStageFlags = &waitStageDst;
		syncObjects.pWaitSemaphores = &renderer.acquiredNextImage;
		syncObjects.waitSemaphoreCount = 1;

		SumbitToVulkanQueue(renderer.graphicsQueue, cmdBuffer, &syncObjects, renderer.finishedRendering);
		
		vkWaitForFences(renderer.device, 1, &renderer.finishedRendering, VK_TRUE, UINT64_MAX);
		vkResetFences(renderer.device, 1, &renderer.finishedRendering);
		PresentVulkanSwapchain(renderer.graphicsQueue, renderer.swapchain.swapchain, imageIndex, nullptr, 0);

		vkFreeCommandBuffers(renderer.device, renderer.commandPool, 1, &cmdBuffer);
	}

	void ShutdownRenderer(Renderer& renderer)
	{
		vkDeviceWaitIdle(renderer.device);

		vkDestroyDescriptorPool(renderer.device, renderer.descriptorPool, nullptr);
		vkDestroyImageView(renderer.device, renderer.spriteImageView, nullptr);
		vkDestroyImage(renderer.device, renderer.spriteImage, nullptr);
		vkDestroyBuffer(renderer.device, renderer.uniformBuffer, nullptr);
		vkDestroySampler(renderer.device, renderer.imageSampler, nullptr);

		DestroyDeviceStackAllocator(renderer.device, renderer.hostMemory);
		DestroyDeviceStackAllocator(renderer.device, renderer.textureMemory);

		vkDestroyPipeline(renderer.device, renderer.spritePipeline.pipeline, nullptr);
		vkDestroyPipelineLayout(renderer.device, renderer.spritePipeline.pipelineLayout, nullptr);
		vkDestroyDescriptorSetLayout(renderer.device, renderer.spritePipeline.setLayout, nullptr);

		vkDestroyShaderModule(renderer.device, renderer.vertexShader, nullptr);
		vkDestroyShaderModule(renderer.device, renderer.fragmentShader, nullptr);

		vkDestroySemaphore(renderer.device, renderer.acquiredNextImage, nullptr);
		vkDestroyFence(renderer.device, renderer.finishedRendering, nullptr);

		vkDestroyCommandPool(renderer.device, renderer.commandPool, nullptr);

		for (U32 i = 0; i < renderer.swapchain.framebufferCount; i++)
		{
			vkDestroyFramebuffer(renderer.device, renderer.swapchain.pFramebuffers[i], nullptr);
			vkDestroyImageView(renderer.device, renderer.swapchain.pImageViews[i], nullptr);
		}
		vkDestroyRenderPass(renderer.device, renderer.swapchain.renderPass, nullptr);
		vkDestroySwapchainKHR(renderer.device, renderer.swapchain.swapchain, nullptr);

		vkDestroyDevice(renderer.device, nullptr);
		vkDestroySurfaceKHR(renderer.instance, renderer.windowSurface, nullptr);
#ifdef DEBUG
		vkDestroyDebugUtilsMessengerEXT(renderer.instance, renderer.debugMessenger, nullptr);
#endif
		vkDestroyInstance(renderer.instance, nullptr);
		volkFinalize();

		free(renderer.swapchain.pFramebuffers);
		free(renderer.swapchain.pImageViews);
	}

	DeviceStackAllocator CreateDeviceStackAllocator(VkDevice device, U32 size, U32 memoryTypeIndex)
	{
		DeviceStackAllocator allocator;
		allocator.currentOffset = 0;
		allocator.size = size;

		VkMemoryAllocateInfo info;
		info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		info.pNext = nullptr;
		info.memoryTypeIndex = memoryTypeIndex;
		info.allocationSize = size;

		vkAllocateMemory(device, &info, nullptr, &allocator.memory);
		return allocator;
	}

	VkResult BindBufferToDeviceStack(VkDevice device, VkBuffer buffer, DeviceStackAllocator& allocator)
	{
		VkMemoryRequirements requirements;
		vkGetBufferMemoryRequirements(device, buffer, &requirements);

		if (allocator.currentOffset + requirements.size > allocator.size)
			return VK_ERROR_OUT_OF_POOL_MEMORY;

		vkBindBufferMemory(device, buffer, allocator.memory, allocator.currentOffset);
		allocator.currentOffset += requirements.size;
		return VK_SUCCESS;
	}

	VkResult BindImageToDeviceStack(VkDevice device, VkImage image, DeviceStackAllocator& allocator)
	{
		VkMemoryRequirements requirements;
		vkGetImageMemoryRequirements(device, image, &requirements);

		if (allocator.currentOffset + requirements.size > allocator.size)
			return VK_ERROR_OUT_OF_POOL_MEMORY;

		VkResult result = vkBindImageMemory(device, image, allocator.memory, allocator.currentOffset);
		allocator.currentOffset += requirements.size;
		return result;
	}

	void DestroyDeviceStackAllocator(VkDevice device, DeviceStackAllocator& allocator)
	{
		vkFreeMemory(device, allocator.memory, nullptr);
	}
}