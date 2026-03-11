#ifndef OSX_WINDOW_H
#define OSX_WINDOW_H

typedef void (*DrawCallbackFn)(float dt);
void CreateAndRunNSApp(void *buf, unsigned int width, unsigned int height, DrawCallbackFn fn);
void InitWindow(void *buf, unsigned int width, unsigned int height, DrawCallbackFn fn);
void ProcessWindowEvents(void);
bool ShouldWindowClose(void);
void CloseWindow(void);

#endif // OSX_WINDOW_H
