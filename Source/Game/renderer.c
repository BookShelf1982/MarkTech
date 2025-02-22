#include "renderer.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#include <glad\glad.h>
#include <glad\glad_wgl.h>
#include <spirv_cross\spirv_cross_c.h>
#include <stdio.h>

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

typedef struct {
	char* source;
	int32_t length;
	uint32_t* data;
	size_t dataSize;
} ShaderData;

void ReadFullFile(const char* filenamae, char** data, size_t* dataSize)
{
	FILE* file = NULL;
	fopen_s(&file, filenamae, "rb");
	if (!file)
		return;
	
	fseek(file, 0, SEEK_END);
	size_t size = ftell(file);
	fseek(file, 0, SEEK_SET);
	*dataSize = size;
	*data = (char*)malloc(size);
	fread(*data, size, 1, file);
	fclose(file);
}

void GenerateGLSLCode(spvc_context context, uint32_t* spvCode, size_t codeSize, char** source)
{
	spvc_compiler compiler = NULL;
	spvc_parsed_ir parsed_ir = NULL;
	spvc_compiler_options options = NULL;
	spvc_context_parse_spirv(context, spvCode, codeSize / 4, &parsed_ir);
	spvc_context_create_compiler(context, SPVC_BACKEND_GLSL, parsed_ir, SPVC_CAPTURE_MODE_TAKE_OWNERSHIP, &compiler);
	spvc_compiler_create_compiler_options(compiler, &options);
	spvc_compiler_options_set_uint(options, SPVC_COMPILER_OPTION_GLSL_VERSION, 460);
	spvc_compiler_options_set_bool(options, SPVC_COMPILER_OPTION_GLSL_ES, SPVC_FALSE);
	spvc_compiler_install_compiler_options(compiler, options);

	spvc_compiler_compile(compiler, source);
}

void CreateRenderer(Renderer* renderer, Window* window, uint32_t flags)
{
	renderer->window = window;
#ifdef _WIN32
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

	HDC dc = GetDC((HWND)window->_handle);
	int format = ChoosePixelFormat(dc, &pfd);
	SetPixelFormat(dc, format, &pfd);

	HGLRC glrcDummy = wglCreateContext(dc); // make a dummy context
	wglMakeCurrent(dc, glrcDummy);

	// load opengl
	gladLoadWGL(dc);
	wglSwapIntervalEXT(flags & 0x1);

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
	
	renderer->context = (uintptr_t)glrcReal;
	renderer->dc = (uintptr_t)dc;
}

void DestroyRenderer(Renderer* renderer)
{
	glDeleteVertexArrays(1, &renderer->vao);
	glDeleteProgram(renderer->shaders[0].program);
	glDeleteShader(renderer->shaders[0].fragShader);
	glDeleteShader(renderer->shaders[0].vertShader);

#ifdef _WIN32
	wglMakeCurrent((HDC)renderer->dc, NULL);
	wglDeleteContext((HGLRC)renderer->context);
	ReleaseDC((HWND)renderer->window->_handle, (HDC)renderer->dc);
#endif
}

void CompileShader(spvc_context context, const char* filename)
{
	ShaderData data;
	char* shaderSource;
	ReadFullFile(filename, &data.data, &data.dataSize);
	GenerateGLSLCode(context, data.data, data.dataSize, &shaderSource);
}

void LoadShaders(Renderer* renderer)
{
	ShaderData vertData;
	ShaderData fragData;

	ReadFullFile("vert.spv", (char**)&vertData.data, &vertData.dataSize);
	ReadFullFile("frag.spv", (char**)&fragData.data, &fragData.dataSize);

	spvc_context context = NULL;
	spvc_context_create(&context);
#ifdef DEBUG
	spvc_context_set_error_callback(context, spvcErrorCallback, NULL);
#endif // DEBUG

	GenerateGLSLCode(context, vertData.data, vertData.dataSize, &vertData.source);
	GenerateGLSLCode(context, fragData.data, fragData.dataSize, &fragData.source);
	vertData.length = (int32_t)strlen(vertData.source);
	fragData.length = (int32_t)strlen(fragData.source);

	free(vertData.data);
	free(fragData.data);

	uint32_t vertShader = glCreateShader(GL_VERTEX_SHADER);
	uint32_t fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	renderer->shaders[0].vertShader = vertShader;
	renderer->shaders[0].fragShader = fragShader;

	glShaderSource(vertShader, 1, &vertData.source, &vertData.length);
	glShaderSource(fragShader, 1, &fragData.source, &fragData.length);
	glCompileShader(vertShader);
	glCompileShader(fragShader);

	spvc_context_release_allocations(context);
	spvc_context_destroy(context);

	uint32_t program = glCreateProgram();
	renderer->shaders[0].program = program;
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);

	glLinkProgram(program);
	glDetachShader(program, vertShader);
	glDetachShader(program, fragShader);

	glUseProgram(program);

	glGenVertexArrays(1, &renderer->vao);
	glBindVertexArray(renderer->vao);
}

void RenderFrame(Renderer* renderer)
{
	glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	SwapBuffers((HDC)renderer->dc);
}
