#pragma once

#include <stdint.h>

#define MAX_ACTIVE_WINDOWS 4

struct Window 
{
	uintptr_t _handle;
};

typedef void (*PFN_WindowClose)();
typedef void (*PFN_WindowKeyChanged)(uint32_t keycode);

void CreateWindow(const char *title, uint32_t width, uint32_t height, Window& window);
void ReleaseWindow(Window& window);
void PollWindowEvents(Window& window);
void SetCloseCallback(Window& window, PFN_WindowClose callback);
void SetKeyPressedCallback(Window& window, PFN_WindowKeyChanged callback);
void SetKeyReleasedCallback(Window& window, PFN_WindowKeyChanged callback);