#include <stdlib.h>
#include <stdbool.h>
#include <float.h>
#include <stdint.h>
#include <assert.h>

#define NOB_STRIP_PREFIX
#include "nob.h"

#include <stdint.h>

#include "graphics.h"

V3f ScreenSpace(V3f p)
{
  float px = p.x / p.z;
  float py = p.y / p.z;
  float x = ((px + 1)/2);
  float y = ((py + 1)/2);
  return v3f(x, y, p.z);
}

Color SampleImage(Image *image, V2f sample_pos)
{
  // [0..1] >> [0..width/height]
  // [0..1] >> [1..0] >>  [height..0]
  unsigned int u = roundf(fmod(sample_pos.x, 1.0) * image->width);
  unsigned int v = roundf((1.0 - fmod(sample_pos.y, 1.0)) * image->height);

  size_t index = (v * image->width) + u;
  index = index > image->width * image->height ? image->width * image->height : index;
  
  Color *buf = image->buf;
  Color ret_color = buf[index];
  return ret_color;
}

void GrPushVertex(GrContext *gc, Vertex v)
{
  da_append(&gc->verts, v);
}

#define NEARCLIP_PLANE 1.0f

float InverseLerp(float a, float b, float c) { return (c - a) / (b - c); }

Vertex ClipVertex(Vertex a, Vertex b, float near)
{
  float dx = b.p.x - a.p.x;
  float dy = b.p.y - a.p.y;
  float dz = b.p.z - a.p.z;
  float dnz = near - a.p.z;
  float y = (dy/dz * dnz) + a.p.y;
  float x = (dx/dz * dnz) + a.p.x;

  float t = InverseLerp(a.p.x, b.p.x, x);
  V2f tc = v2f_lerp(a.tc, b.tc, v2ff(t));
  
  return (Vertex) {
    .p = v3f(x, y, near),
    .tc = tc
  };
}

extern bool breakpoint;

bool IsBackface(V3f a, V3f b, V3f c)
{
  V3f edge0 = v3f_sub(b, a);
  V3f edge1 = v3f_sub(c, a);
  
  float det = (edge0.x * edge1.y) - (edge0.y * edge1.x);
  
  return det < 0;
}

void PushTriangle(GrContext *gc, Vertex v1, Vertex v2, Vertex v3)
{
  if (IsBackface(ScreenSpace(v1.p), ScreenSpace(v2.p), ScreenSpace(v3.p))) return;
  GrPushVertex(gc, v1);
  GrPushVertex(gc, v2);
  GrPushVertex(gc, v3);
}

void GrTriangle(GrContext *gc, Vertex v1, Vertex v2, Vertex v3)
{
  Vertex vertices[3] = {v1, v2, v3};
  
  float d0 = v1.p.z;
  float d1 = v2.p.z;
  float d2 = v3.p.z;
  
  int behind_verts[3] = {0};
  int behind_count = 0;
  int front_verts[3] = {0};
  int front_count = 0;
  
  if (d0 < NEARCLIP_PLANE) { behind_verts[behind_count++] = 0; } else { front_verts[front_count++] = 0; }
  if (d1 < NEARCLIP_PLANE) { behind_verts[behind_count++] = 1; } else { front_verts[front_count++] = 1; }
  if (d2 < NEARCLIP_PLANE) { behind_verts[behind_count++] = 2; } else { front_verts[front_count++] = 2; }
  
  switch (behind_count) {
  case 0: {
    PushTriangle(gc, v1, v2, v3);
  } return;
  case 1: {
    Vertex a = vertices[front_verts[0]];
    Vertex b = vertices[front_verts[1]];
    Vertex c = vertices[behind_verts[0]];

    if (front_verts[0] == 0 && front_verts[1] == 2) {
      Vertex temp = a;
      a = b;
      b = temp;
    }
    
    Vertex d = ClipVertex(c, a, NEARCLIP_PLANE);
    Vertex new_c = ClipVertex(c, b, NEARCLIP_PLANE);
    V2f tc = v2f(1, 0);
    a.tc = tc;
    b.tc = tc;
    new_c.tc = tc;
    d.tc = tc;

    PushTriangle(gc, a, b, new_c);
    PushTriangle(gc, a, new_c, d);
  } return;
  case 2: {
    Vertex a = vertices[front_verts[0]];
    Vertex b = vertices[behind_verts[0]];
    Vertex c = vertices[behind_verts[1]];
    
    if (behind_verts[0] == 0 && behind_verts[1] == 2) {
      Vertex temp = b;
      b = c;
      c = temp;
    }
    
    Vertex b_prime = ClipVertex(b, a, NEARCLIP_PLANE);
    Vertex c_prime = ClipVertex(c, a, NEARCLIP_PLANE);
    V2f tc = v2f(1, 1);
    a.tc = tc;
    b_prime.tc = tc;
    c_prime.tc = tc;
    
    PushTriangle(gc, a, b_prime, c_prime);
  } return;
  case 3:
    break;
  };
}

#define Max3(a,b,c) ((a > b) ? ((a > c) ? a : c) : ((b > c) ? b : c))
#define Min3(a,b,c) ((a < b) ? ((a < c) ? a : c) : ((b < c) ? b : c))

bool debug_draw = false;

// [-1..1] coordinate space, with (0, 0) in the center of the framebuffer.
// Winding order is counter clockwise

void GrTriangleFill(GrContext *gc, Vertex v1, Vertex v2, Vertex v3)
{
  v1.p = ScreenSpace(v1.p);
  v2.p = ScreenSpace(v2.p);
  v3.p = ScreenSpace(v3.p);

  int width = gc->framebuffer->width;
  int height = gc->framebuffer->height;

  V3f scale = v3f(width, height, 1);
  V3i v1p = v3i3f(v3f_mul(v1.p, scale));
  V3i v2p = v3i3f(v3f_mul(v2.p, scale));
  V3i v3p = v3i3f(v3f_mul(v3.p, scale));

  V3i edge0 = v3i_sub(v2p, v1p);
  V3i edge1 = v3i_sub(v3p, v2p);
  V3i edge2 = v3i_sub(v1p, v3p);
  
  V3i v1v3 = v3i_sub(v3p, v1p);
  V3i v1v2 = v3i_sub(v2p, v1p);
  int det = (v1v2.x * v1v3.y) - (v1v2.y * v1v3.x);
  if (det < 0) return;
  
  int min_x = Min3(v1p.x, v2p.x, v3p.x); 
  int min_y = Max3(v1p.y, v2p.y, v3p.y);
  int max_x = Max3(v1p.x, v2p.x, v3p.x);
  int max_y = Min3(v1p.y, v2p.y, v3p.y);

  if (max_x > width) max_x = width; else if (max_x < 0) max_x = 0;
  if (min_x < 0) min_x = 0; else if (min_x > width) min_x = width;
  if (max_y > height) max_y = height; else if (max_y < 0) max_y = 0;
  if (min_y < 0) min_y = 0; else if (min_y > height) min_y = height;

  min_y = height - min_y;
  max_y = height - max_y;
  
  assert(min_y <= max_y);
  assert(min_x <= max_x);
  
  for (int i = min_y; i < max_y; i++) {
    for (int j = min_x; j < max_x; j++) { 
      int x = j;
      int y = height - i;
      int u = (edge1.x * (y-v2p.y)) - (edge1.y * (x-v2p.x));
      int v = (edge2.x * (y-v3p.y)) - (edge2.y * (x-v3p.x));
      int w = (edge0.x * (y-v1p.y)) - (edge0.y * (x-v1p.x));

      bool inside = true;
      inside &= u == 0 ? ((edge1.x > 0 && edge1.y == 0) || edge1.y < 0) : u > 0;
      inside &= v == 0 ? ((edge2.x > 0 && edge2.y == 0) || edge2.y < 0) : v > 0;
      inside &= w == 0 ? ((edge0.x > 0 && edge0.y == 0) || edge0.y < 0) : w > 0;

      if (debug_draw) gc->framebuffer->buf[j + (i * gc->framebuffer->width)] = (Color) {255, 0, 255, 255};
      if (!inside) continue;
      
      float z = 1.0f / ((((1.0f/v1.p.z)*u)/det) + (((1.0f/v2.p.z)*v)/det) + (((1.0f/v3.p.z)*w)/det));
      unsigned int z_int = (unsigned int)((z / 100.0f) * (float)0xFFFFFFFF);
      unsigned int z_buffer_z = gc->depth_buffer->buf[j + (i * gc->depth_buffer->width)];
      if (z_int > z_buffer_z) continue;
      gc->depth_buffer->buf[j + (i * gc->depth_buffer->width)] = z_int;
      
      V2f tcoord = v2f(
        (v1.tc.x*u)/det + (v2.tc.x*v)/det + (v3.tc.x*w)/det,
        (v1.tc.y*u)/det + (v2.tc.y*v)/det + (v3.tc.y*w)/det
      );
      
      //Color color = SampleImage(gc->sampled_texture, tcoord);
      Color color = {tcoord.x*255, tcoord.y*255, 0, 255};
      gc->framebuffer->buf[j + (i * gc->framebuffer->width)] = color;
    }
  }
}

void GrClear(GrContext *gc)
{
  for (size_t i = 0; i < gc->framebuffer->width * gc->framebuffer->height; i++) {
    gc->framebuffer->buf[i] = (Color) {0, 0, 0, 255};
  }
  
  for (size_t i = 0; i < gc->framebuffer->width * gc->framebuffer->height; i++) {
    gc->depth_buffer->buf[i] = 0xFFFFFFFF;
  }
}

void GrFlush(GrContext *gc) {
  for (size_t i = 0; i < gc->verts.count; i += 3) {
    GrTriangleFill(gc, gc->verts.items[i], gc->verts.items[i+1], gc->verts.items[i+2]);
  }
  gc->verts.count = 0;
}
