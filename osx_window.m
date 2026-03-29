#include <stdio.h>
#include <stdbool.h>
#import <AppKit/AppKit.h>
#import <QuartzCore/QuartzCore.h>
#import <QuartzCore/CoreAnimation.h>
#include <time.h>
#include "osx_window.h"
#include "osx_scancodes.h"

static unsigned int fb_width, fb_height = 0;
static void *fb = NULL;
static DrawFn draw_callback = NULL;
static bool should_close = false;

typedef struct {
  bool repeat;
  bool down;
} KeyState;

static KeyState keymap[SCANCODE_COUNT] = {0};

typedef struct {
  float x, y;
  float dx, dy;
} MouseState;

static MouseState mouse = {0};

@interface LayerDelegate : NSObject <CALayerDelegate>
@end

@implementation LayerDelegate
- (void) displayLayer:(CALayer *) layer
{
  const size_t bits_per_comp = 8;
  const size_t bits_per_pixel = 32;
  const size_t bytes_per_row = 4*fb_width;
  CGColorSpaceRef color_space = CGColorSpaceCreateWithName(kCGColorSpaceSRGB);
  CGBitmapInfo info = kCGBitmapByteOrder32Big | kCGImageAlphaLast;
  CGDataProviderRef provider =  CGDataProviderCreateWithData(NULL, fb, bytes_per_row * fb_height, NULL);

  CGImageRef img = CGImageCreate(fb_width, fb_height, bits_per_comp, bits_per_pixel,
                                 bytes_per_row, color_space, info, provider, NULL,
                                 NO, kCGRenderingIntentDefault);
  [CATransaction begin];
  [CATransaction setValue:(id)kCFBooleanTrue
                   forKey:kCATransactionDisableActions];
  
  layer.contents = (id)img;

  [CATransaction commit];

  CGImageRelease(img);
  CGDataProviderRelease(provider);
}
@end

@interface GameApp : NSApplication
- (void)terminate:(id)sender;
- (void)sendEvent:(NSEvent *)theEvent;
@end

@implementation GameApp
- (void)terminate:(id)sender
{
  printf("stfu\n");
}

- (void)sendEvent:(NSEvent *)theEvent
{
  [super sendEvent:theEvent];
}
@end

@interface GameWindowDelegate : NSObject <NSWindowDelegate>
@end

@implementation GameWindowDelegate
- (void) windowWillClose:(NSNotification *) notification
{
  should_close = true;
}
@end

@interface GameView : NSView
@end

@implementation GameView
- (BOOL)acceptsFirstResponder {
    return YES;
}
@end

@interface GameWindow : NSWindow
- (BOOL)canBecomeKeyWindow;
- (BOOL)canBecomeMainWindow;
- (void)sendEvent:(NSEvent *)event;
- (void)doCommandBySelector:(SEL)aSelector;
@end

@implementation GameWindow
- (BOOL)canBecomeKeyWindow
{
  return YES;
}

- (BOOL)canBecomeMainWindow
{
  return YES;
}

- (void)sendEvent:(NSEvent *)event
{
  [super sendEvent:event];
}

- (void)doCommandBySelector:(SEL)aSelector {}
@end

static GameWindow *window;
static CALayer *layer;
static CADisplayLink *display_link;

@interface AppDelegate : NSObject <NSApplicationDelegate>
@end

@implementation AppDelegate
- (void) applicationWillFinishLaunching:(NSNotification *) notification
{
  int width = fb_width;
  int height = fb_height;

  NSRect rect = NSMakeRect(0, 0, width, height);
  window = [[GameWindow alloc]
                  initWithContentRect:rect
                            styleMask:NSWindowStyleMaskTitled |
                                      NSWindowStyleMaskMiniaturizable |
                                      NSWindowStyleMaskClosable |
                                      NSWindowStyleMaskResizable
                              backing:NSBackingStoreBuffered
                                defer:false];
  window.contentView.wantsLayer = YES;
  window.delegate = [[GameWindowDelegate alloc] init];

  window.contentView = [[GameView alloc] init];

  layer = [[CALayer alloc] init];
  layer.magnificationFilter = kCAFilterNearest;
  layer.autoresizingMask = kCALayerNotSizable;
  layer.contentsGravity = kCAGravityResizeAspect;
  
  window.contentView.layer = layer;
  
  layer.delegate = [[LayerDelegate alloc] init];
  //NSRect layer_rect = layer.frame;

  display_link = [window displayLinkWithTarget:self
                                      selector:@selector(step:)];
  CAFrameRateRange range = {
    .minimum = 10.0f,
    .maximum = 30.0f
  };
  display_link.preferredFrameRateRange = range;
  
  [display_link addToRunLoop:[NSRunLoop currentRunLoop]
                          forMode:NSRunLoopCommonModes];
  
  [window center];
  [window makeKeyAndOrderFront:nil];

}

- (void)step:(CADisplayLink *)sender
{
  draw_callback();
  [window.contentView.layer setNeedsDisplay];
}
@end

void InitWindow(void *buf, unsigned int width, unsigned int height, DrawFn fn)
{
  fb = buf;
  fb_width = width;
  fb_height = height;
  draw_callback = fn;
  @autoreleasepool {
    [GameApp sharedApplication];
    AppDelegate *app_delegate = [[AppDelegate alloc] init];
    [NSApp setDelegate:app_delegate];
    [NSApp setPresentationOptions:NSApplicationPresentationDefault];
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
    [NSApp finishLaunching];
  }
}

bool ShouldWindowClose(void) { return should_close; }

void ProcessWindowEvents(void)
{
  for (;;) {
    NSEvent *event = [NSApp nextEventMatchingMask:NSEventMaskAny untilDate:nil inMode:NSDefaultRunLoopMode dequeue:YES];
    switch (event.type) {
      case NSEventTypeKeyUp: {
        KeyState *s = &keymap[osx_scancode_table[event.keyCode]];
        s->down = false;
        s->repeat = false;
      } return;
      case NSEventTypeKeyDown: {
        KeyState *s = &keymap[osx_scancode_table[event.keyCode]];
        if (s->down) {
          s->repeat = true;
        }
        s->down = true;
      } return;
      case NSEventTypeMouseMoved: {
        NSPoint loc = [NSEvent mouseLocation];
        mouse.x = loc.x;
        mouse.y = loc.y;
        mouse.dx = [event deltaX];
        mouse.dy = [event deltaY];
      } return;
      default: break;
    }
      
    if (event == nil) return;
    [NSApp sendEvent:event];
  }
}

bool IsKeyPressed(int scancode)
{
  bool pressed = (keymap[scancode].down) && (!keymap[scancode].repeat);
  if (pressed) keymap[scancode].repeat = true;
  return pressed;
}
bool IsKeyDown(int scancode) { return keymap[scancode].down; }
bool IsKeyUp(int scancode) { return !keymap[scancode].down; }

void GetMousePos(float *x, float *y)
{
  *x = mouse.x;
  *y = mouse.y;
}

void GetMouseDelta(float *x, float *y)
{
  *x = mouse.dx;
  *y = mouse.dy;
}

void CloseWindow(void)
{
  [window close];
}

uint64_t GetHighResTime(void) { return clock_gettime_nsec_np(CLOCK_MONOTONIC); }
