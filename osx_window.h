#ifndef OSX_WINDOW_H
#define OSX_WINDOW_H

#include <stdint.h>

typedef void (*DrawFn)(void);
typedef void (*ResizeBufFn)(void **ptr, int *width, int *height, int window_width, int window_height);

void InitWindow(void *buf, unsigned int width, unsigned int height, DrawFn draw_fn);
void ProcessWindowEvents(void);
bool ShouldWindowClose(void);
void CloseWindow(void);

bool IsKeyDown(int scancode);
bool IsKeyUp(int scancode);
bool IsKeyPressed(int scancode);

bool IsLeftMouseBtnDown(void);
void GetMousePos(float *x, float *y);
void GetMouseDelta(float *x, float *y);

uint64_t GetHighResTime(void);

#endif // OSX_WINDOW_H
