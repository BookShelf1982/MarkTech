#ifdef MT_PLATFORM_WINDOWS
#include <Windows.h>
#endif
#ifdef DEBUG
#include <crtdbg.h>
#endif

#include <Window.h>
#include <GraphicsInterface.h>
#include <Log.h>
#include <File.h>
#include <HashMap.h>
#include <nbt.h>

using namespace MarkTech;

bool gIsRunning = true;

void WindowEventHandler(void* pEvent)
{
	WindowEventType event = (WindowEventType) *(WindowEventType*)pEvent;
	switch (event)
	{
	case WINDOW_EVENT_CLOSE:
	{
		gIsRunning = false;
	} return;
	case WINDOW_EVENT_KEYCHANGED:
	{
		WindowEventKeyChangedInfo event = *(WindowEventKeyChangedInfo*)pEvent;
		if (event.keydown && event.keycode == VK_ESCAPE)
			gIsRunning = false;

	} return;
	}
}

void ReadSPVFile(const char* pFilepath, ShaderCreateInfo& info)
{
	File file;
	FOpen(&file, pFilepath, FILE_ACCESS_READ);
	if (!file.isOpened || (file.size % 4) != 0)
		return;

	info.sizeInBytes = (U32)file.size;
	info.pSPIR = (U32*)malloc(file.size);
	FRead(&file, (char*)info.pSPIR, file.size);
	FClose(&file);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	nbt_tag is_tag;
	nbt_tag age_tag;
	nbt_create_tag_info_t create_tag_info;
	create_tag_info.name = "is_tag";
	create_tag_info.payload.byteTag = 1;
	create_tag_info.type = NBT_TAG_TYPE_BYTE;
	nbt_create_tag(&create_tag_info, &is_tag);
	
	create_tag_info.name = "age";
	create_tag_info.payload.intTag = 27;
	create_tag_info.type = NBT_TAG_TYPE_INT;
	nbt_create_tag(&create_tag_info, &age_tag);

	nbt_tag compound_tag;
	create_tag_info.name = "compound";
	create_tag_info.payload = {};
	create_tag_info.type = NBT_TAG_TYPE_COMPOUND;
	nbt_create_tag(&create_tag_info, &compound_tag);
	nbt_compound_add_tag(compound_tag, is_tag);
	nbt_compound_add_tag(compound_tag, age_tag);

	nbt_compound_remove_tag(compound_tag, "age");

	nbt_destroy_tag(is_tag);
	nbt_destroy_tag(compound_tag);
	nbt_destroy_tag(age_tag);

	/*Window window;
	WindowInfo windowInfo;
	windowInfo.defaultMode = WINDOW_MODE_BORDERLESS_WINDOWED;
	windowInfo.pTitle = "MarkTech Window";
	windowInfo.width = 640;
	windowInfo.height = 480;
	windowInfo.pfnEventHandler = WindowEventHandler;

	ConstructWindow(windowInfo, window);

	ApplicationInfo appInfo;
	appInfo.pName = "MarkTech";
	appInfo.verMajor = 1;
	appInfo.verMinor = 0;
	appInfo.verPatch = 0;

	GraphicsContextCreateInfo info;
	info.api = GRAPHICS_API_VULKAN;
	info.flags = 0;
	info.pAppInfo = &appInfo;
#ifdef MT_PLATFORM_WINDOWS
	info.flags |= CONTEXT_FLAGS_DEBUG_MESSAGES;
#endif

	GraphicsContext context = nullptr;
	CreateGraphicsContext(info, &context);

	SwapchainCreateInfo swapchainInfo;
	swapchainInfo.pWindow = &window;
	swapchainInfo.oldSwapchain = nullptr;
	swapchainInfo.presentationMode = PRESENTATION_MODE_FIFO;

	Swapchain swapchain = nullptr;
	CreateSwapchain(context, swapchainInfo, &swapchain);

	CommandBuffer commandBuffer = nullptr;
	CreateCommandBuffer(context, &commandBuffer);

	Shader vertShader = nullptr;
	Shader fragShader = nullptr;

	ShaderCreateInfo vertInfo;
	ReadSPVFile("vert.spv", vertInfo);

	ShaderCreateInfo fragInfo;
	ReadSPVFile("frag.spv", fragInfo);

	CreateShader(context, vertInfo, &vertShader);
	CreateShader(context, fragInfo, &fragShader);

	free(vertInfo.pSPIR);
	free(fragInfo.pSPIR);

	GraphicsPipeline mainPipeline = nullptr;

	RasterizationInfo rasterInfo;
	rasterInfo.cullMode = CULL_MODE_BACK;
	rasterInfo.frontFace = FRONT_FACE_CW;
	rasterInfo.lineWidth = 1.0f;
	rasterInfo.polygonMode = POLYGON_MODE_FILL;

	VertexAttribute posAttrib;
	posAttrib.location = 0;
	posAttrib.offset = 0;
	posAttrib.componentLength = 3;

	VertexInputInfo vertInput;
	vertInput.pAttribs = &posAttrib;
	vertInput.attribCount = 1;
	vertInput.stride = 12;

	FramebufferClass swapchainFbClass = nullptr;
	GetSwapchainFramebufferClass(swapchain, &swapchainFbClass);

	ShaderStage vertStage;
	vertStage.shader = vertShader;
	vertStage.stage = SHADER_PIPELINE_STAGE_VERTEX;
	vertStage.pEntrypoint = "main";

	ShaderStage fragStage;
	fragStage.shader = fragShader;
	fragStage.stage = SHADER_PIPELINE_STAGE_FRAGMENT;
	fragStage.pEntrypoint = "main";

	ShaderStage stages[] = { vertStage, fragStage };

	GraphicsPipelineCreateInfo pipelineInfo;
	pipelineInfo.topology = TOPOLOGY_TRIANGLE_LIST;
	pipelineInfo.pVertexInfos = nullptr;
	pipelineInfo.vertexInfoCount = 0;
	pipelineInfo.pRasterizationInfo = &rasterInfo;
	pipelineInfo.framebufferClass = swapchainFbClass;
	pipelineInfo.pStages = stages;
	pipelineInfo.stageCount = sizeof(stages) / sizeof(ShaderStage);

	CreateGraphicsPipeline(context, pipelineInfo, &mainPipeline);

	Extent2D swapchainExtent;
	GetSwapchainExtent(swapchain, &swapchainExtent);
	
	Viewport view;
	view.x = 0.0f;
	view.y = 0.0f;
	view.width = (F32)swapchainExtent.width;
	view.height = (F32)swapchainExtent.height;
	view.minDepth = 0.0f;
	view.maxDepth = 1.0f;

	Rect2D scissor;
	scissor.x = 0;
	scissor.y = 0;
	scissor.width = swapchainExtent.width;
	scissor.height = swapchainExtent.height;
	
	Framebuffer currentFramebuffer = nullptr;
	while (gIsRunning)
	{
		PollWindowMessages();
		ResetCommandBuffer(commandBuffer);
		AcquireNextSwapchainImage(context, swapchain, &currentFramebuffer);
		StartCommandRecording(commandBuffer);
		CmdBeginRenderTarget(commandBuffer, currentFramebuffer);
		CmdBindPipeline(commandBuffer, mainPipeline);
		CmdSetViewport(commandBuffer, view);
		CmdSetScissor(commandBuffer, scissor);
		CmdDrawVertices(commandBuffer, 0, 3);
		CmdEndRenderTarget(commandBuffer);
		FinishCommandBuffer(commandBuffer);
		SubmitCommandBuffer(context, commandBuffer);
		SwapchainPresent(context, swapchain);
	}

	WaitDeviceIdle(context);
	DestroyGraphicsPipeline(context, mainPipeline);
	DestroyShader(context, vertShader);
	DestroyShader(context, fragShader);
	DestroyCommandBuffer(context, commandBuffer);
	DestroySwapchain(context, swapchain);
	DestroyGraphicsContext(context);
	ReleaseWindow(window);
	
	ShutdownLog();*/
#ifdef DEBUG
	_CrtDumpMemoryLeaks();
#endif
	return 0;
}
