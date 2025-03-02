#include "renderer.h"

#ifdef _WINDOWS
#include <Windows.h>
#endif

#include "alloc_uitls.h"

#include <glad\glad.h>
#include <glad\glad_wgl.h>
#include <spirv_glsl.hpp>

#include <stdio.h>
#include <FileSystem.h>
#include <string.h>

#ifdef DEBUG
void APIENTRY DebugCallback (GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	printf("%s\n", message);
}

void spvcErrorCallback(void* userdata, const char* error)
{
	printf("%s\n", error);
}
#endif // DEBUG

std::string GenerateGLSLCode(uint32_t* spvCode, size_t codeSize, spv::ExecutionModel* execModel)
{
	spirv_cross::CompilerGLSL compiler(spvCode, codeSize / 4);
	spirv_cross::CompilerGLSL::Options options;
	*execModel = compiler.get_execution_model();

	options.es = false;
	options.version = 460;
	options.separate_shader_objects = true;
	compiler.set_common_options(options);
	return compiler.compile();
}

void CreateRenderer(const RendererSettings& settings, Renderer& renderer)
{
	renderer.window = settings.window;

#ifdef _WINDOWS
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
		PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
		32,                   // Colordepth of the framebuffer.
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,                   // Number of bits for the depthbuffer
		8,                    // Number of bits for the stencilbuffer
		0,                    // Number of Aux buffers in the framebuffer.
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	HDC dc = GetDC((HWND)settings.window->_handle);
	int format = ChoosePixelFormat(dc, &pfd);
	SetPixelFormat(dc, format, &pfd);

	HGLRC glrcDummy = wglCreateContext(dc); // make a dummy context
	wglMakeCurrent(dc, glrcDummy);

	// load opengl
	gladLoadWGL(dc);
	wglSwapIntervalEXT(settings.flags & 0x1);

	const int attribList[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 6,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB, 32,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,
#ifdef DEBUG
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
#endif
		0, // End
	};

	HGLRC glrcReal = wglCreateContextAttribsARB(dc, NULL, attribList);
	wglMakeCurrent(dc, glrcReal);
	wglDeleteContext(glrcDummy);
#endif

	gladUnloadWGL();
	gladLoadGL();

#ifdef DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(DebugCallback, NULL); 
#endif // DEBUG
	
	renderer.context = (uintptr_t)glrcReal;
	renderer.dc = (uintptr_t)dc;

	glCreateBuffers(1, &renderer.ubo);
	glNamedBufferData(renderer.ubo, sizeof(16) * 32, nullptr, GL_STREAM_DRAW);

	glCreateVertexArrays(1, &renderer.vao);
	glCreateProgramPipelines(1, &renderer.ppo);
	glClearColor(0.0f, 0.0f, 0.45f, 1.0f);
	glBindVertexArray(renderer.vao);
}

void DestroyRenderer(Renderer& renderer)
{
	for (size_t i = 0; i < renderer.shaders.count; i++)
	{
		glDeleteProgram(renderer.shaders.elements[i]);
	}
	
	glDeleteBuffers(1, &renderer.ubo);
	glDeleteProgramPipelines(1, &renderer.ppo);
	glDeleteVertexArrays(1, &renderer.vao);

#ifdef _WINDOWS
	wglMakeCurrent((HDC)renderer.dc, NULL);
	wglDeleteContext((HGLRC)renderer.context);
	ReleaseDC((HWND)renderer.window->_handle, (HDC)renderer.dc);
#endif
}

uint32_t LoadCompileShader(Renderer& renderer, const LoadShaderInfo& info)
{
	spv::ExecutionModel model;
	std::string src = GenerateGLSLCode((uint32_t*)info.binary, info.binarySize, &model);

	GLenum shaderType = 0;
	switch (model)
	{
	case spv::ExecutionModelVertex:
		shaderType = GL_VERTEX_SHADER;
		break;
	case spv::ExecutionModelFragment:
		shaderType = GL_FRAGMENT_SHADER;
		break;
	}

	const char* srcs[] = { src.c_str() };

	uint32_t program = glCreateShaderProgramv(shaderType, 1, srcs);

	int32_t status = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &status);

	if (status)
	{
		printf("Successfuly compiled shader!\n");
	}

	renderer.shaders[renderer.shaders.count] = program;
	renderer.shaders.count++;

	return (uint32_t)renderer.shaders.count - 1;
}

bool LoadShader(Renderer& renderer, const LoadShaderInfo& info, uint32_t* id)
{
	uint32_t program = 0;
	GLenum binaryType = *(GLenum*)info.binary;
	program = glCreateProgram();
	glProgramParameteri(program, GL_PROGRAM_SEPARABLE, GL_TRUE);
	glProgramBinary(program, binaryType, info.binary + 4, (GLsizei)info.binarySize - 4);
	
	int32_t status = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (!status)
	{
		printf("Failed to load shader!");
		return false;
	}

	renderer.shaders[renderer.shaders.count] = program;
	renderer.shaders.count++;

	if (id)
		*id = (uint32_t)renderer.shaders.count - 1;

	return true;
}

void CreatePipeline(Renderer& renderer)
{
	glUniformBlockBinding(renderer.shaders[0], 0, 0);
	glUseProgramStages(renderer.ppo, GL_VERTEX_SHADER_BIT, renderer.shaders[0]);
	glUseProgramStages(renderer.ppo, GL_FRAGMENT_SHADER_BIT, renderer.shaders[1]);
}

void RenderFrame(Renderer& renderer, SceneSnapshot& snapshot)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glNamedBufferSubData(renderer.ubo, 0, snapshot.origins.size * sizeof(vec3f_t), snapshot.origins.elements);
	glBindProgramPipeline(renderer.ppo);

	for (size_t i = 0; i < snapshot.models.size; i++)
	{
		glBindBufferRange(GL_UNIFORM_BUFFER, 0, renderer.ubo, i * sizeof(vec3f_t), sizeof(vec3f_t));
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	snapshot.models.size = 0;
	snapshot.origins.size = 0;
}

void PresentFrame(Renderer& renderer)
{
	SwapBuffers((HDC)renderer.dc);
}

////////////////////////////
//// POST LOAD PFNS ///////
//////////////////////////
bool PostLoadShaderBinary(uint32_t id, const char* filepath, const Resource const* resource, void* user)
{
	LoadShaderInfo info = {};
	info.binary = resource->data;
	info.binarySize = resource->dataSize;
	info.binaryType = BINARY_TYPE_COMPILED;

	Renderer& r = *static_cast<Renderer*>(user);
	return LoadShader(r, info, nullptr);
}

bool PostLoadShader(uint32_t id, const char* filepath, const Resource const* resource, void* user)
{
	LoadShaderInfo info = {};
	info.binary = resource->data;
	info.binarySize = resource->dataSize;
	info.binaryType = BINARY_TYPE_SPIRV;

	Renderer& r = *static_cast<Renderer*>(user);
	uint32_t shader = LoadCompileShader(r, info);
	uint32_t binaryFormat = 0;
	int32_t binarySize = 0;

	glGetProgramiv(r.shaders[shader], GL_PROGRAM_BINARY_LENGTH, &binarySize);

	char* binary = (char*)AlignedAlloc(binarySize, 4);
	glGetProgramBinary(r.shaders[shader], binarySize, &binarySize, &binaryFormat, binary);

	File file;
	Path path;
	strcpy_s(path.path, sizeof(path.path), filepath);
	FSRenameExtension(&path, ".cache");
	FSOpen(&path, OPEN_TYPE_WRITE, &file);
	if (!file._handle)
		return true;

	FSWrite(&file, (char*)&binaryFormat, sizeof(uint32_t));
	FSWrite(&file, (char*)binary, binarySize);
	FSClose(&file);

	AlignedFree(binary);
	return true;
}

void SetPostLoadFns(ResourceManager& manager, Renderer& renderer)
{
	manager.postLoadFns[RESOURCE_TYPE_SHADER].fn = PostLoadShader;
	manager.postLoadFns[RESOURCE_TYPE_SHADER].user = &renderer;

	manager.postLoadFns[RESOURCE_TYPE_SHADER_BINARY].fn = PostLoadShaderBinary;
	manager.postLoadFns[RESOURCE_TYPE_SHADER_BINARY].user = &renderer;
}
