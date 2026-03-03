#include <stdio.h>
#import <AppKit/AppKit.h>
#import <QuartzCore/QuartzCore.h>
#import <QuartzCore/CoreAnimation.h>
#include "osx_window.h"

static DrawCallbackFn draw_callback = NULL;
static unsigned int fb_width, fb_height = 0;
static void *fb = NULL;

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
  
  layer.contents = (id)img;
  CGImageRelease(img);
  CGDataProviderRelease(provider);
}
@end

@interface AppDelegate : NSObject <NSApplicationDelegate>
@property (nonatomic, strong) NSWindow *window;
@property (nonatomic, strong) CALayer *layer;
@property (nonatomic, strong) CADisplayLink *display_link;
@end

@implementation AppDelegate
- (void) applicationWillFinishLaunching:(NSNotification *) notification
{
  int width = fb_width;
  int height = fb_height;

  NSRect rect = NSMakeRect(0, 0, width, height);
  self.window = [[NSWindow alloc]
                  initWithContentRect:rect
                            styleMask:NSWindowStyleMaskTitled |
                                      NSWindowStyleMaskMiniaturizable |
                                      NSWindowStyleMaskClosable |
                                      NSWindowStyleMaskResizable
                              backing:NSBackingStoreBuffered
                                defer:false];
  self.window.contentView.wantsLayer = YES;

  self.layer = [[CALayer alloc] init];
  self.layer.magnificationFilter = kCAFilterNearest;
  self.layer.autoresizingMask = kCALayerWidthSizable | kCALayerHeightSizable;
  self.layer.contentsGravity = kCAGravityResizeAspectFill;
  
  self.window.contentView.layer = self.layer;
  
  self.layer.delegate = [[LayerDelegate alloc] init];
  //NSRect layer_rect = self.layer.frame;

  self.display_link = [self.window displayLinkWithTarget:self
                                                selector:@selector(step:)];
  
  [self.display_link addToRunLoop:[NSRunLoop currentRunLoop]
                          forMode:NSRunLoopCommonModes];
  
  [self.window center];
  [self.window makeKeyAndOrderFront:nil];

}

- (void)step:(CADisplayLink *)sender
{
  draw_callback((float)sender.duration);
  [self.window.contentView.layer setNeedsDisplay];
}

- (BOOL) applicationShouldTerminateAfterLastWindowClosed:(NSApplication *) sender
{
  return YES;
}
@end

void CreateAndRunNSApp(void *buf, unsigned int width, unsigned int height, DrawCallbackFn fn)
{
  fb = buf;
  fb_width = width;
  fb_height = height;
  draw_callback = fn;
  @autoreleasepool {
    [NSApplication sharedApplication];
    AppDelegate *app_delegate = [[AppDelegate alloc] init];
    [NSApp setDelegate:app_delegate];
    [NSApp setPresentationOptions:NSApplicationPresentationDefault];
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];

    [NSApp run];
  }
}   
