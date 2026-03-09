#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "xdmath.h"

typedef struct {
  D3 p;
  D2 tc;
} Vertex;

typedef struct {
  unsigned char r, g, b, a;
} Color;

typedef enum {
  IMAGE_FORMAT_RGBA8,
  IMAGE_FORMAT_R32,
  IMAGE_FORMAT_MAX_ENUM = 0xFFFFFFFF
} ImageFormat;

typedef struct {
  ImageFormat format;
  unsigned int width, height;
  void *buf;
} Image;

typedef struct {
  unsigned int width, height;
  Color *buf;
} Framebuffer;

typedef struct {
  unsigned int width, height;
  unsigned int *buf;
} DepthBuffer;

typedef struct {
  Framebuffer *framebuffer;
  DepthBuffer *depth_buffer;
  Image *sampled_texture;
  M4x4 transform;
} GrContext;

#define CreateImage(type, width, height)              \
do {                                                  \
  .buf = malloc(sizeof((type)) * (width) * (height)), \
  .width = (width),                                   \
  .height = (height)                                  \
} while (0)

D3 ScreenSpace(GrContext *gc, D3 p);
D3 TrimD4(D4 v);
D4 MakeD4FromD3(D3 v, float w);

void GrTriangle(GrContext *gc, Vertex v1, Vertex v2, Vertex v3);
void GrClear(GrContext *gc);

#endif // GRAPHICS_H
