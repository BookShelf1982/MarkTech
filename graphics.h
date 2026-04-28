#ifndef GRAPHICS_H
#define GRAPHICS_H

#define LA_IMPLEMENTATION
#include "la.h"

typedef struct {
  V3f p;
  V3f n;
  V2f tc;
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
  Vertex *items;
  size_t count;
  size_t capacity;
} VertexList;

typedef enum {
  RENDER_MODE_LAMBERT,
  RENDER_MODE_FLAT_AMBIENT
} RenderMode;

typedef struct {
  Framebuffer *framebuffer;
  DepthBuffer *depth_buffer;
  Image *sampled_texture;
  VertexList verts;
  RenderMode mode;
  float light_power;
  float aspect_ratio;
} GrContext;

#define CreateImage(type, width, height)              \
do {                                                  \
  .buf = malloc(sizeof((type)) * (width) * (height)), \
  .width = (width),                                   \
  .height = (height)                                  \
} while (0)

void GrTriangle(GrContext *gc, Vertex v1, Vertex v2, Vertex v3);
void GrFlush(GrContext *gc);
void GrClear(GrContext *gc);

#endif // GRAPHICS_H
