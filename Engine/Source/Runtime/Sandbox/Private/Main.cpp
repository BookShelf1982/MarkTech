#ifdef MT_PLATFORM_WINDOWS
#include <Windows.h>
#endif

#include "PrimitiveTypes\intalias.h"
#include "Time\Time.h"
#include "Threads\Thread.h"
#include "Threads\Mutex.h"
#include "Window\Window.h"
#include "Renderer\Renderer.h"
#include "Math3D\Math3D.h"
#include <ini.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <stdio.h>
#include <time.h>
#ifdef DEBUG
#include <crtdbg.h>
#endif

/*
* Objective: Make pac man
*/

using namespace MarkTech;

bool g_isClosing = false;
const U64 g_tickLength = 15;

#define BUTTON_UP 0x1
#define BUTTON_DOWN 0x2
#define BUTTON_LEFT 0x4
#define BUTTON_RIGHT 0x8
#define BUTTON_SELECT 0x10

struct InputState 
{
	U8 buttons;
};

InputState g_inputState = {};

struct Entity
{
	Vector2 pos;
	U32 resourceId;
	U32 spriteId;
};

#define NUM_GHOST 5
#define ENTITY_RADIUS 0.5f

struct GameState
{
	bool buttonDown;
	I32 rot;
	Vector2 wishDir;
	Entity player;
	Entity ghosts[NUM_GHOST];
};

GameState g_gameState = {};

void WindowHandler(WindowEventHandle wndEvent)
{
	WindowEventType type = *(WindowEventType*)wndEvent;
	switch (type)
	{
	case WINDOW_EVENT_TYPE_CLOSE:
		g_isClosing = true;
		return;
	case WINDOW_EVENT_TYPE_KEYDOWN:
	{
		WindowKeyChangedEvent* pEvent = (WindowKeyChangedEvent*)wndEvent;
		switch (pEvent->keycode)
		{
		case VK_UP:
			g_inputState.buttons |= BUTTON_UP;
			break;
		case VK_DOWN:
			g_inputState.buttons |= BUTTON_DOWN;
			break;
		case VK_LEFT:
			g_inputState.buttons |= BUTTON_LEFT;
			break;
		case VK_RIGHT:
			g_inputState.buttons |= BUTTON_RIGHT;
			break;
		case VK_SPACE:
			g_inputState.buttons |= BUTTON_SELECT;
			break;
		case VK_ESCAPE:
			g_isClosing = true;
		}
	} return;
	case WINDOW_EVENT_TYPE_KEYUP:
	{
		WindowKeyChangedEvent* pEvent = (WindowKeyChangedEvent*)wndEvent;
		switch (pEvent->keycode)
		{
		case VK_UP:
			g_inputState.buttons = (g_inputState.buttons & ~(1 << (1 - 1)));
			break;
		case VK_DOWN:
			g_inputState.buttons = (g_inputState.buttons & ~(1 << (2 - 1)));
			break;
		case VK_LEFT:
			g_inputState.buttons = (g_inputState.buttons & ~(1 << (3 - 1)));
			break;
		case VK_RIGHT:
			g_inputState.buttons = (g_inputState.buttons & ~(1 << (4 - 1)));
			break;
		case VK_SPACE:
			g_inputState.buttons = (g_inputState.buttons & ~(1 << (5 - 1)));
			break;
		}
	} return;
	}
}

void TickPlayerMovement(float dt)
{
	g_gameState.wishDir = { 0.0f, 0.0f };
	if (g_inputState.buttons & BUTTON_RIGHT)
	{
		g_gameState.wishDir.x += 1.0f;
	}
	if (g_inputState.buttons & BUTTON_LEFT)
	{
		g_gameState.wishDir.x -= 1.0f;
	}
	if (g_inputState.buttons & BUTTON_UP)
	{
		g_gameState.wishDir.y += 1.0f;
	}
	if (g_inputState.buttons & BUTTON_DOWN)
	{
		g_gameState.wishDir.y -= 1.0f;
	}
	if (g_inputState.buttons & BUTTON_SELECT)
	{
		if (!g_gameState.buttonDown)
		{
			g_gameState.buttonDown = true;

			g_gameState.rot += 90;
			if (g_gameState.rot >= 360)
			{
				g_gameState.rot = 0;
			}
		}
	}
	else
	{
		g_gameState.buttonDown = false;
	}

	Vector2 moveVec = NormalizeV2(g_gameState.wishDir) * 10000.0f * dt;

	g_gameState.player.pos = AddV2(g_gameState.player.pos, moveVec);
	g_gameState.ghosts[0].pos = AddV2(g_gameState.ghosts[0].pos, { -moveVec.x, -moveVec.y });
}

void TickCollisions()
{
	for (U32 i = 0; i < NUM_GHOST; i++)
	{
		Vector2 distVec = SubtractV2(g_gameState.player.pos, g_gameState.ghosts[i].pos);
		float distance = MagnitudeV2(distVec);
		if (distance <= ENTITY_RADIUS * 2)
			g_gameState.player.pos = AddV2(g_gameState.player.pos, NormalizeV2(distVec));
	}
}

void CreateSnapshot(SceneSnapshot& snapshot)
{
	if (snapshot.elements.size > 0)
		snapshot.elements.size = 0;

	SceneElement player;
	player.sprite = g_gameState.player.spriteId;
	player.image = g_gameState.player.resourceId;
	player.world = Translate3x3(g_gameState.player.pos.x, -g_gameState.player.pos.y);

	InsertArrayItem(snapshot.elements, player);

	for (U32 i = 0; i < NUM_GHOST; i++)
	{
		SceneElement ghost;
		ghost.sprite = g_gameState.ghosts[i].spriteId;
		ghost.image = g_gameState.ghosts[i].resourceId;
		ghost.world = Translate3x3(g_gameState.ghosts[i].pos.x, -g_gameState.ghosts[i].pos.y);
		InsertArrayItem(snapshot.elements, ghost);
	}
}

void TickGameState(float dt, SceneSnapshot& snapshot) 
{
	TickPlayerMovement(dt);
	TickCollisions();
	CreateSnapshot(snapshot);
}

typedef struct {
	char playerImage[200];
	char ghostImage[200];
} assets_t;

#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
int IniHandler(void* user, const char* section, const char* name, const char* value)
{
	if (MATCH("Game.Startup", "playerImage"))
	{
		strcpy(((assets_t*)user)->playerImage, value);
	}
	if (MATCH("Game.Startup", "ghostImage"))
	{
		strcpy(((assets_t*)user)->ghostImage, value);
	}
	return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	assets_t assets = {};

	ini_parse("Config.ini", IniHandler, &assets);

	StartTime();
	double lastTime = GetTime();

	WindowProps props;
	props.width = 800;
	props.height = 600;
	props.pTitle = "Pac-Man";
	props.style = WINDOW_STYLE_BORDERLESS;
	props.eventHandler = WindowHandler;
	Window window = MakeWindow(props);

	RendererConfig rConfig;
	rConfig.enableVSync = false;
	rConfig.pWindow = &window;
	rConfig.textureFiltering = TEXTURE_FILTERING_BILINEAR;

	Renderer renderer;
	if (!InitRenderer(rConfig, renderer))
	{
		WindowMessageBox(window, "MarkTech Error", "Failed to initialize renderer", WINDOW_MESSAGE_TYPE_ERROR);
		return 1;
	}

	// TODO: Upload sprite textures to GPU
	I32 width, height, channels = 0;
	U8* pData = stbi_load(assets.playerImage, &width, &height, &channels, 4);

	UploadTextureInfo textureInfo;
	textureInfo.channels = channels;
	textureInfo.dataSize = 4 * width * height;
	textureInfo.width = width;
	textureInfo.height = height;
	textureInfo.pData = pData;

	U32 playerTextureId = UploadTextureToRenderer(renderer, textureInfo);
	U32 playerSpriteId = CreateSpriteDescriptor(renderer, playerTextureId);
	stbi_image_free(pData);

	g_gameState.player.resourceId = playerTextureId;
	g_gameState.player.spriteId = playerSpriteId;

	pData = stbi_load(assets.ghostImage, &width, &height, &channels, 4);

	textureInfo.channels = channels;
	textureInfo.dataSize = 4 * width * height;
	textureInfo.width = width;
	textureInfo.height = height;
	textureInfo.pData = pData;

	U32 ghostTextureId = UploadTextureToRenderer(renderer, textureInfo);
	stbi_image_free(pData);

	for (U32 i = 0; i < NUM_GHOST; i++)
	{
		g_gameState.ghosts[i].resourceId = ghostTextureId;
		g_gameState.ghosts[i].spriteId = CreateSpriteDescriptor(renderer, ghostTextureId);
	}

	Matrix3x3 projection = OrthoProjection3x3(0.0f, 16.0f, 0.0f, 9.0f);
	UpdateFrameUniform(renderer, &projection);

	SceneSnapshot snapshot;
	snapshot.elements = ReserveArray<SceneElement>(16);

	g_gameState.player.pos = { 16, -9 };
	g_gameState.ghosts[0].pos = { 0, 0 };

	while (true) 
	{
		double startTime = GetTime();
		PollWindowEvents(window);
		
		const bool isClosing = g_isClosing;
		if (isClosing)
			break;

		float dt = (float)(startTime - lastTime) / 1000.0f;

		TickGameState(dt, snapshot);

		RenderFrame(renderer, snapshot);
		lastTime = startTime;
	}

#ifdef DEBUG
	char buffer[64] = "";
	sprintf_s(buffer, "Player entity: Pos: {  %f,  %f } Rot: %i\n", g_gameState.player.pos.x, g_gameState.player.pos.y, g_gameState.rot);
	OutputDebugStringA(buffer);
#endif

	DestroyArray(snapshot.elements);
	ShutdownRenderer(renderer);
	KillWindow(window);

#ifdef DEBUG
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}