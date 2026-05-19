#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#import <AppKit/AppKit.h>
#import <QuartzCore/QuartzCore.h>
#import <QuartzCore/CoreAnimation.h>
#import <AVFoundation/AVFoundation.h>
#include <time.h>
#include "osx_window.h"
#include "osx_scancodes.h"

static unsigned int fb_width, fb_height;
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
  int buttons;
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
- (BOOL)acceptsFirstResponder
{
  return YES;
}

- (BOOL) performKeyEquivalent:(NSEvent *) event
{
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
static GameView *view;
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
  window.acceptsMouseMovedEvents = YES;
  window.delegate = [[GameWindowDelegate alloc] init];

  view = [[GameView alloc] init];
  window.contentView = view;

  layer = [[CALayer alloc] init];
  layer.magnificationFilter = kCAFilterNearest;
  layer.autoresizingMask = kCALayerNotSizable;
  layer.contentsGravity = kCAGravityResizeAspect;
  
  window.contentView.layer = layer;
  
  layer.delegate = [[LayerDelegate alloc] init];

  display_link = [window displayLinkWithTarget:self
                                      selector:@selector(step:)];
  CAFrameRateRange range = {
    .minimum = 10.0f,
    .maximum = 60.0f
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

void InitWindow(void *buf, unsigned int width, unsigned int height, DrawFn draw_fn)
{
  fb = buf;
  fb_width = width;
  fb_height = height;
  draw_callback = draw_fn;
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

void UpdateMouse(NSEvent *event)
{
  NSPoint loc = [NSEvent mouseLocation];
  loc = [window convertPointFromScreen:loc];
  loc = [view convertPoint:loc fromView:nil];
  loc = [layer convertPoint:loc fromLayer:nil];

  CGRect bounds = [layer bounds];
  CGSize framebuffer_size = CGSizeMake(fb_width, fb_height);
  CGRect scaled_content = AVMakeRectWithAspectRatioInsideRect(framebuffer_size, bounds);

  CGSize bs = bounds.size;
  CGSize scs = scaled_content.size;

  int nudge_x = bs.width  > scs.width  ? (bs.width  - scs.width)  / 2 : 0;
  int nudge_y = bs.height > scs.height ? (bs.height - scs.height) / 2 : 0;
  
  mouse.x = loc.x - nudge_x;
  mouse.y = loc.y - nudge_y;

  mouse.x /= scs.width;
  mouse.y /= scs.height;
  
  switch (event.type) {
  case NSEventTypeLeftMouseDown:
    mouse.buttons |= 1;
    break;
  case NSEventTypeLeftMouseUp:
    mouse.buttons &= ~(1 << 0);
    break;
  default: break;
  }
}

void ProcessWindowEvents(void)
{
  for (;;) {
    NSEvent *event = [NSApp nextEventMatchingMask:NSEventMaskAny untilDate:nil inMode:NSDefaultRunLoopMode dequeue:YES];
    if (event == nil) return;
    
    switch (event.type) {
    case NSEventTypeKeyUp: {
      KeyState *s = &keymap[osx_scancode_table[event.keyCode]];
      s->down = false;
      s->repeat = false;
      break;
    }
    case NSEventTypeKeyDown: {
      KeyState *s = &keymap[osx_scancode_table[event.keyCode]];
      if (s->down) {
        s->repeat = true;
      }
      s->down = true;
      break;
    }
    case NSEventTypeLeftMouseDown:
    case NSEventTypeLeftMouseUp:
    case NSEventTypeRightMouseDown:
    case NSEventTypeRightMouseUp:
    case NSEventTypeMouseMoved:
    case NSEventTypeLeftMouseDragged:
    case NSEventTypeRightMouseDragged:
    case NSEventTypeMouseEntered:
    case NSEventTypeMouseExited:
    case NSEventTypeOtherMouseUp:
    case NSEventTypeOtherMouseDragged:
      UpdateMouse(event);
      break;
    
    default: break;
    }
    
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

bool IsLeftMouseBtnDown() { return (mouse.buttons & 1); }

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
