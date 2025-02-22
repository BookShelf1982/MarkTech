#ifndef WINDOW_H
#define WINDOW_H

#include <stdint.h>

#define MAX_ACTIVE_WINDOWS 4

typedef struct {
	uintptr_t _handle;
} Window;
typedef void (*PFN_windowClose)();

void CreateWindow(const char *title, uint32_t width, uint32_t height, Window* window);
void ReleaseWindow(Window* window);
void PollWindowEvents(Window* window);
void SetCloseCallback(Window* window, PFN_windowClose callback);

#endif // !WINDOW_H