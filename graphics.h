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

typedef struct {
  float max_x, max_y;
  float min_x, min_y;
} BoundingBox;

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
} GrContext;

#define CreateImage(type, width, height)              \
do {                                                  \
  .buf = malloc(sizeof((type)) * (width) * (height)), \
  .width = (width),                                   \
  .height = (height)                                  \
} while (0)

void DrawIt(float dt);

#endif // GRAPHICS_H
