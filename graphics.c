#include <stdlib.h>
#include <stdbool.h>
#include <float.h>
#include <stdint.h>
#include "graphics.h"
#include "xdmath.h"

D3 ScreenSpace(GrContext *gc, D3 p)
{
  float px = p.x / p.z;
  float py = p.y / p.z;
  float x = (((px + 1)/2)*gc->framebuffer->width) + 0.5;
  float y = (((-py + 1)/2)*gc->framebuffer->height) + 0.5;
  return (D3) {x, y, p.z};
}

Color SampleImage(Image *image, D2 sample_pos)
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

D3 TrimD4(D4 v) { return (D3) {v.x, v.y, v.z}; }
D4 MakeD4FromD3(D3 v, float w) { return (D4) {v.x, v.y, v.z, w}; }

#define Max3(a,b,c) ((a > b) ? ((a > c) ? a : c) : ((b > c) ? b : c))
#define Min3(a,b,c) ((a < b) ? ((a < c) ? a : c) : ((b < c) ? b : c))

// [-1..1] coordinate space, with (0, 0) in the center of the framebuffer.
// Winding order is counter clockwise
void GrTriangle(GrContext *gc, Vertex v1, Vertex v2, Vertex v3)
{
  D3i v1p = (D3i) {v1.p.x, v1.p.y, v1.p.z};
  D3i v2p = (D3i) {v2.p.x, v2.p.y, v2.p.z};
  D3i v3p = (D3i) {v3.p.x, v3.p.y, v3.p.z};
  D3i edge0 = (D3i) {v2p.x - v1p.x, v2p.y - v1p.y, v2p.z - v1p.z};
  D3i edge1 = (D3i) {v3p.x - v2p.x, v3p.y - v2p.y, v3p.z - v2p.z};
  D3i edge2 = (D3i) {v1p.x - v3p.x, v1p.y - v3p.y, v1p.z - v3p.z};
  int det = (edge0.x * edge1.y) - (edge0.y * edge1.x);
  if (det < 0) return;

  int min_x = Min3(v1p.x, v2p.x, v3p.x); 
  int min_y = Min3(v1p.y, v2p.y, v3p.y);
  int max_x = Max3(v1p.x, v2p.x, v3p.x);
  int max_y = Max3(v1p.y, v2p.y, v3p.y);

  if (max_x > gc->framebuffer->width) max_x = gc->framebuffer->width;
  if (min_x < 0.0f) min_x = 0.0f;
  if (min_y < 0.0f) min_y = 0.0f;
  if (max_y > gc->framebuffer->height) max_y = gc->framebuffer->height;
  
  for (int i = min_y; i < max_y; i++) {
    for (int j = min_x; j < max_x; j++) { 
      int x = j;
      int y = i;
      int u = (edge1.x * (y-v2p.y)) - (edge1.y * (x-v2p.x));
      int v = (edge2.x * (y-v3p.y)) - (edge2.y * (x-v3p.x));
      int w = (edge0.x * (y-v1p.y)) - (edge0.y * (x-v1p.x));

      // test if on top or left edge or inside triangle
      
      //bool inside = (u >= 0 && v >= 0 && w >= 0);
      bool inside = true;
      inside &= u == 0 ? ((edge1.x > 0 && edge1.y == 0) || edge1.y < 0) : u > 0;
      inside &= v == 0 ? ((edge2.x > 0 && edge2.y == 0) || edge2.y < 0) : v > 0;
      inside &= w == 0 ? ((edge0.x > 0 && edge0.y == 0) || edge0.y < 0) : w > 0;
      if (!inside) continue;
      
      float z = 1.0f / ((((1.0f/v1.p.z)*u)/det) + (((1.0f/v2.p.z)*v)/det) + (((1.0f/v3.p.z)*w)/det));
      unsigned int z_int = (unsigned int)((z / 100.0f) * (float)0xFFFFFFFF);
      unsigned int z_buffer_z = gc->depth_buffer->buf[j + (i * gc->depth_buffer->width)];
      if (z_int > z_buffer_z) continue;
      gc->depth_buffer->buf[j + (i * gc->depth_buffer->width)] = z_int;
      
      D2 tcoord = (D2){
        (v1.tc.x*u)/det + (v2.tc.x*v)/det + (v3.tc.x*w)/det,
        (v1.tc.y*u)/det + (v2.tc.y*v)/det + (v3.tc.y*w)/det
      };
      
      Color color = SampleImage(gc->sampled_texture, tcoord);
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
