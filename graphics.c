#include <stdlib.h>
#include <stdbool.h>
#include "graphics.h"
#include "xdmath.h"

float SignedArea(D3 v1, D3 v2)
{
  return (v1.x * v2.y) - (v1.y * v2.x);
}

void TriangleBounds(
  D3 a,
  D3 b,
  D3 c,
  BoundingBox *box
)
{
  box->max_x = (a.x > b.x) ? ((a.x > c.x) ? a.x : c.x) : ((b.x > c.x) ? b.x : c.x);
  box->min_x = (a.x < b.x) ? ((a.x < c.x) ? a.x : c.x) : ((b.x < c.x) ? b.x : c.x);
  box->max_y = (a.y > b.y) ? ((a.y > c.y) ? a.y : c.y) : ((b.y > c.y) ? b.y : c.y);
  box->min_y = (a.y < b.y) ? ((a.y < c.y) ? a.y : c.y) : ((b.y < c.y) ? b.y : c.y);
}

D3 TranslatePoint(D3 p, D3 t)
{
  return (D3) {p.x + t.x, p.y + t.y, p.z + t.z};
}

D3 RotatePointZAxis(D3 p, float a)
{
  float sin_a = sinf(a);
  float cos_a = cosf(a);
  float x = (p.x * cos_a) - (p.y * sin_a);
  float y = (p.x * sin_a) + (p.y * cos_a);
  float z = p.z;
  return (D3) {x, y, z};
}

D3 RotatePointYAxis(D3 p, float a)
{
  float sin_a = sinf(a);
  float cos_a = cosf(a);
  float x = (p.x * cos_a) - (p.z * sin_a);
  float y = p.y;
  float z = (p.x * sin_a) + (p.z * cos_a);
  return (D3) {x, y, z};
}

D3 ScreenSpace(GrContext *gc, D3 p)
{
  float aspect = (float)gc->framebuffer->height / (float)gc->framebuffer->width; 
  float px = p.x / p.z;
  float py = p.y / p.z;
  float x = (((px * aspect) + 1.0f) / 2.0f) *  gc->framebuffer->width;
  float y = ((-py + 1.0f) / 2.0f) * gc->framebuffer->height;
  return (D3) {x, y, p.z};
}

D3 SampleImage(Image *image, D2 sample_pos)
{
  // [0..1] >> [0..width/height]
  // [0..1] >> [1..0] >>  [height..0]
  int u = roundf(fmod(sample_pos.x, 1.0) * image->width);
  int v = roundf((1.0 - fmod(sample_pos.y, 1.0)) * image->height);
  v = v == image->height ? image->height - 1 : v;

  size_t index = (v * image->width) + u;
  if (index > image->width * image->height) {
    return (D3) {
      1.0f,
      0.0f,
      1.0f
    };
  }
  Color *buf = image->buf;
  Color ret_color = buf[index];
  return (D3) {
    ret_color.r / 255.0,
    ret_color.g / 255.0,
    ret_color.b / 255.0,
  };
}

// [-1..1] coordinate space, with (0, 0) in the center of the framebuffer.
void GrTriangle(GrContext *gc, Vertex v1, Vertex v2, Vertex v3)
{
  BoundingBox box = {0};
  v1.p = ScreenSpace(gc, v1.p);
  v2.p = ScreenSpace(gc, v2.p);
  v3.p = ScreenSpace(gc, v3.p);

  TriangleBounds(v1.p, v2.p, v3.p, &box);

  if (box.max_x > gc->framebuffer->width) box.max_x = gc->framebuffer->width;
  if (box.min_x < 0.0f) box.min_x = 0.0f;
  if (box.min_y < 0.0f) box.min_y = 0.0f;
  if (box.max_y > gc->framebuffer->height) box.max_y = gc->framebuffer->height;
  
  D3 v1v2 = (D3){v2.p.x - v1.p.x, v2.p.y - v1.p.y, v2.p.z - v1.p.z};
  D3 v1v3 = (D3){v3.p.x - v1.p.x, v3.p.y - v1.p.y, v3.p.z - v1.p.z};
  float area = SignedArea(v1v2, v1v3) / 2.0f;
  if (area < 0.0f) return;
 
  for (int i = (int)box.min_y; i < (int)box.max_y; i++) {
    for (int j = (int)box.min_x; j < (int)box.max_x; j++) { 
      D3 v1p = (D3){((((float)j) + 0.5f) - v1.p.x), ((((float)i) + 0.5f) - v1.p.y), v1.p.z};

      float v = (SignedArea(v1p, v1v3) / 2.0f) / area;
      float w = (SignedArea(v1v2, v1p) / 2.0f) / area;
      float u = 1.0f - v - w;

      bool inside = (0.0f <= u && u <= 1.0f) && (0.0f <= w && w <= 1.0f) && (0.0f <= v && v <= 1);
      if (!inside) continue;
      
      float z = 1.0f / ((u * (1.0f/v1.p.z)) + (v * (1.0f/v2.p.z)) + (w * (1.0f/v3.p.z)));
      unsigned int z_int = (unsigned int)((z / 100.0f) * (float)0xFFFFFFFF);
      unsigned int z_buffer_z = gc->depth_buffer->buf[j + (i * gc->depth_buffer->width)];
      if (z_int > z_buffer_z) continue;
      gc->depth_buffer->buf[j + (i * gc->depth_buffer->width)] = z_int;
      
      D2 tcoord = (D2){
        u * v1.tc.x + v * v2.tc.x + w * v3.tc.x,
        u * v1.tc.y + v * v2.tc.y + w * v3.tc.y
      };

      D3 color = SampleImage(gc->sampled_texture, tcoord);
      
      gc->framebuffer->buf[j + (i * gc->framebuffer->width)] = (Color){
        (unsigned char)(color.x * 255.0f),
        (unsigned char)(color.y * 255.0f),
        (unsigned char)(color.z * 255.0f),
        255
      };
    }
  }
}

typedef struct {
  Vertex v1, v2, v3;
} Triangle;

Triangle DefaultTriangle(void)
{
  const float sqrt_3 = 1.73205080757f;
  const float s = 1.0f;
  return (Triangle) {
    .v1 = {
      .p = {0.0f, (s*sqrt_3) / 3.0f, 0.0f},
      .tc = {0.0f, 1.0f}
    },
    .v2 = {
      .p = {-s/2.0f, -(s*sqrt_3/6.0f), 0.0f},
      .tc = {0.0f, 0.0f}
    },
    .v3 = {
      .p = {s/2.0f, -(s*sqrt_3/6.0f), 0.0f},
      .tc = {1.0f, 0.0f}
    }
  };
}

float theta = 0;

extern GrContext gc;

void DrawIt(float dt)
{
  for (size_t i = 0; i < width * height; i++) {
    gc.framebuffer->buf[i] = (Color) {0, 0, 0, 255};
  }

  for (size_t i = 0; i < width * height; i++) {
    gc.depth_buffer->buf[i] = 0xFFFFFFFF;
  }
}
