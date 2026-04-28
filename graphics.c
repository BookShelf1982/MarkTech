#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "gamma_lut.c"

#define NOB_STRIP_PREFIX
#include "nob.h"

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
  unsigned int u = roundf(fmod(sample_pos.x, 1.0) * (image->width - 1));
  unsigned int v = roundf((1.0 - fmod(sample_pos.y, 1.0)) * (image->height - 1));

  size_t index = (v * image->width) + u;
  if (!(index < (image->width * image->height))) printf("tc = (%d, %d)\n", u, v);
  assert(index < (image->width * image->height));
  
  Color *buf = image->buf;
  Color ret_color = buf[index];
  return ret_color;
}

void GrPushVertex(GrContext *gc, Vertex v)
{
  da_append(&gc->verts, v);
}

#define NEARCLIP_PLANE 1.0f

float InverseLerp(float a, float b, float c) { return (c - a) / (b - a); }

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
  V3f n = v3f_lerp(a.n, b.n, v3ff(t));
  
  return (Vertex) {
    .p = v3f(x, y, near),
    .n = n,
    .tc = tc
  };
}

extern bool breakpoint;

bool IsBackface(V3f a, V3f b, V3f c)
{
  V3f edge0 = v3f_sub(b, a);
  V3f edge1 = v3f_sub(c, a);
  
  float det = (edge0.x * edge1.y) - (edge0.y * edge1.x);
  
  return det <= 0;
}

void PushTriangle(GrContext *gc, Vertex v1, Vertex v2, Vertex v3)
{
  v1.p = ScreenSpace(v1.p);
  v2.p = ScreenSpace(v2.p);
  v3.p = ScreenSpace(v3.p);
  if (IsBackface(v1.p, v2.p, v3.p)) return;
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
  
  int behind_v[3] = {0};
  int behind_count = 0;
  int front_v[3] = {0};
  int front_count = 0;
  
  if (d0 < NEARCLIP_PLANE) { behind_v[behind_count++] = 0; } else { front_v[front_count++] = 0; }
  if (d1 < NEARCLIP_PLANE) { behind_v[behind_count++] = 1; } else { front_v[front_count++] = 1; }
  if (d2 < NEARCLIP_PLANE) { behind_v[behind_count++] = 2; } else { front_v[front_count++] = 2; }
  
  switch (behind_count) {
  case 0: {
    PushTriangle(gc, v1, v2, v3);
  } return;
  case 1: {
    Vertex a = vertices[front_v[0]];
    Vertex b = vertices[front_v[1]];
    Vertex c = vertices[behind_v[0]];

    if (front_v[0] == 0 && front_v[1] == 2) {
      Vertex temp = a;
      a = b;
      b = temp;
    }
    
    Vertex d = ClipVertex(c, a, NEARCLIP_PLANE);
    Vertex new_c = ClipVertex(c, b, NEARCLIP_PLANE);

    PushTriangle(gc, a, b, new_c);
    PushTriangle(gc, a, new_c, d);
  } return;
  case 2: {
    Vertex a = vertices[front_v[0]];
    Vertex b = vertices[behind_v[0]];
    Vertex c = vertices[behind_v[1]];
    
    if (behind_v[0] == 0 && behind_v[1] == 2) {
      Vertex temp = b;
      b = c;
      c = temp;
    }
    
    Vertex b_prime = ClipVertex(b, a, NEARCLIP_PLANE);
    Vertex c_prime = ClipVertex(c, a, NEARCLIP_PLANE);
    
    PushTriangle(gc, a, b_prime, c_prime);
  } return;
  case 3:
    break;
  };
}

#define Max3(a,b,c) ((a > b) ? ((a > c) ? a : c) : ((b > c) ? b : c))
#define Min3(a,b,c) ((a < b) ? ((a < c) ? a : c) : ((b < c) ? b : c))

bool debug_draw = false;

typedef struct {
  Vertex v[3];
  V3i vp[3];
  V3i edges[3];
  int det;
  int min_x;
  int max_x;
  int min_y;
  int max_y;
} RasterData;


// [-1..1] coordinate space, with (0, 0) in the center of the framebuffer.
// Winding order is counter clockwise

bool VertexStage(GrContext *gc, RasterData *rd);

inline bool VertexStage(GrContext *gc, RasterData *rd)
{
  int width = gc->framebuffer->width;
  int height = gc->framebuffer->height;

  V3f scale = v3f(width, height, 1);
  rd->vp[0] = v3i3f(v3f_mul(rd->v[0].p, scale));
  rd->vp[1] = v3i3f(v3f_mul(rd->v[1].p, scale));
  rd->vp[2] = v3i3f(v3f_mul(rd->v[2].p, scale));

  rd->edges[0] = v3i_sub(rd->vp[1], rd->vp[0]);
  rd->edges[1] = v3i_sub(rd->vp[2], rd->vp[1]);
  rd->edges[2] = v3i_sub(rd->vp[0], rd->vp[2]);
  
  rd->det = (rd->edges[0].x * rd->edges[1].y) - (rd->edges[0].y * rd->edges[1].x);
  if (rd->det <= 0) return false;
  
  int min_x = Min3(rd->vp[0].x, rd->vp[1].x, rd->vp[2].x); 
  int min_y = Max3(rd->vp[0].y, rd->vp[1].y, rd->vp[2].y);
  int max_x = Max3(rd->vp[0].x, rd->vp[1].x, rd->vp[2].x);
  int max_y = Min3(rd->vp[0].y, rd->vp[1].y, rd->vp[2].y);

  if (max_x > width) max_x = width; else if (max_x < 0) max_x = 0;
  if (min_x < 0) min_x = 0; else if (min_x > width) min_x = width;
  if (max_y > height) max_y = height; else if (max_y < 0) max_y = 0;
  if (min_y < 0) min_y = 0; else if (min_y > height) min_y = height;

  min_y = height - min_y;
  max_y = height - max_y;

  rd->min_x = min_x; 
  rd->min_y = min_y; 
  rd->max_x = max_x; 
  rd->max_y = max_y; 
  
  return true;
}

bool RasterPoint(GrContext *gc, RasterData *rd, int i, int j, int *u, int *v, int *w, float *z);

inline bool RasterPoint(GrContext *gc, RasterData *rd, int i, int j, int *u0, int *u1, int *u2, float *zptr)
{
  V2i p = v2i(j, (gc->framebuffer->height - i) - 1);
  int u = (rd->edges[1].x * (p.y-rd->vp[1].y)) - (rd->edges[1].y * (p.x-rd->vp[1].x));
  int v = (rd->edges[2].x * (p.y-rd->vp[2].y)) - (rd->edges[2].y * (p.x-rd->vp[2].x));
  int w = (rd->edges[0].x * (p.y-rd->vp[0].y)) - (rd->edges[0].y * (p.x-rd->vp[0].x));

  bool inside = true;
  inside &= u == 0 ? ((rd->edges[1].x > 0 && rd->edges[1].y == 0) || rd->edges[1].y < 0) : u > 0;
  inside &= v == 0 ? ((rd->edges[2].x > 0 && rd->edges[2].y == 0) || rd->edges[2].y < 0) : v > 0;
  inside &= w == 0 ? ((rd->edges[0].x > 0 && rd->edges[0].y == 0) || rd->edges[0].y < 0) : w > 0;

  if (!inside) return false;
  
  float z = 1 / ((((1/rd->v[0].p.z)*u)/rd->det) + (((1/rd->v[1].p.z)*v)/rd->det) + (((1/rd->v[2].p.z)*w)/rd->det));
  unsigned int z_int = (unsigned int)((z / 100.0f) * (float)0xFFFFFFFF);
  unsigned int z_buffer_z = gc->depth_buffer->buf[j + (i * gc->depth_buffer->width)];
  if (z_int > z_buffer_z) return false;
  gc->depth_buffer->buf[j + (i * gc->depth_buffer->width)] = z_int;

  *u0 = u;
  *u1 = v;
  *u2 = w;
  *zptr = z;
  
  return true;
}

void AmbientFunction(GrContext *gc, RasterData *rd, int i, int j, int u, int v, int w, float z);

inline void AmbientFunction(GrContext *gc, RasterData *rd, int i, int j, int u, int v, int w, float z)
{
  Vertex v1 = rd->v[0];
  Vertex v2 = rd->v[1];
  Vertex v3 = rd->v[2];
  int det = rd->det;
  
  V2f tc = v2f(
    z * (((v1.tc.x*u)/(v1.p.z))/det + ((v2.tc.x*v)/(v2.p.z))/det + ((v3.tc.x*w)/(v3.p.z))/det),
    z * (((v1.tc.y*u)/(v1.p.z))/det + ((v2.tc.y*v)/(v2.p.z))/det + ((v3.tc.y*w)/(v3.p.z))/det)
  );

  Color tex_color = SampleImage(gc->sampled_texture, tc);
  V3f tex_vec = v3f_div(v3f(rgamma_lut[tex_color.r], rgamma_lut[tex_color.g], rgamma_lut[tex_color.b]), v3ff(255));

  V3f ambient = v3f(0.5, 0.5, 0.5);

  V3f final = v3f_mul(tex_vec, ambient);

  final = v3f_clamp(final, v3ff(0), v3ff(1));
  final = v3f_mul(final, v3ff(255));
  
  final.x = gamma_lut[(int)round(final.x)];
  final.y = gamma_lut[(int)round(final.y)];
  final.z = gamma_lut[(int)round(final.z)];
  
  Color color = (Color) {V3f_Arg(final), 255};
  gc->framebuffer->buf[j + (i * gc->framebuffer->width)] = color;
}

void AmbientTriangleFill(GrContext *gc, Vertex v1, Vertex v2, Vertex v3)
{
  RasterData rd = {0};
  rd.v[0] = v1;
  rd.v[1] = v2;
  rd.v[2] = v3;
  
  if (!VertexStage(gc, &rd)) return;
  
  for (int i = rd.min_y; i < rd.max_y; i++) {
    for (int j = rd.min_x; j < rd.max_x; j++) { 
      int u, v, w;
      float z;
      if (!RasterPoint(gc, &rd, i, j, &u, &v, &w, &z)) continue;
      AmbientFunction(gc, &rd, i, j, u, v, w, z);
    }
  }
}

void LambertFunction(GrContext *gc, RasterData *rd, int i, int j, int u, int v, int w, float z, float *cos_thetas);

inline void LambertFunction(GrContext *gc, RasterData *rd, int i, int j, int u, int v, int w, float z, float *cos_thetas)
{
  Vertex v1 = rd->v[0];
  Vertex v2 = rd->v[1];
  Vertex v3 = rd->v[2];
  int det = rd->det;
  
  V2f tc = v2f(
    z * (((v1.tc.x*u)/(v1.p.z))/det + ((v2.tc.x*v)/(v2.p.z))/det + ((v3.tc.x*w)/(v3.p.z))/det),
    z * (((v1.tc.y*u)/(v1.p.z))/det + ((v2.tc.y*v)/(v2.p.z))/det + ((v3.tc.y*w)/(v3.p.z))/det)
  );
  
  float cos_theta_int = z * (((cos_thetas[0]*u)/v1.p.z)/det + ((cos_thetas[1]*v)/v2.p.z)/det + ((cos_thetas[2]*w)/v3.p.z)/det);
  
  Color tex_color = SampleImage(gc->sampled_texture, tc);
  V3f tex_vec = v3f_div(v3f(rgamma_lut[tex_color.r], rgamma_lut[tex_color.g], rgamma_lut[tex_color.b]), v3ff(255));
  
  Color fb_color = gc->framebuffer->buf[j + (i * gc->framebuffer->width)];
  V3f fb_vec = v3f_div(v3f(rgamma_lut[fb_color.r], rgamma_lut[fb_color.g], rgamma_lut[fb_color.b]), v3ff(255));
  
  V3f final = v3f_add(v3f_mul(tex_vec, v3ff(cos_theta_int*gc->light_power)), fb_vec);
  
  final = v3f_clamp(final, v3ff(0), v3ff(1));      
  final = v3f_mul(final, v3ff(255));

  final.x = gamma_lut[(int)round(final.x)];
  final.y = gamma_lut[(int)round(final.y)];
  final.z = gamma_lut[(int)round(final.z)];
  
  Color color = (Color) {V3f_Arg(final), 255};
  gc->framebuffer->buf[j + (i * gc->framebuffer->width)] = color;
}

void LambertTriangleFill(GrContext *gc, Vertex v1, Vertex v2, Vertex v3)
{
  RasterData rd = {0};
  rd.v[0] = v1;
  rd.v[1] = v2;
  rd.v[2] = v3;
  
  if (!VertexStage(gc, &rd)) return;

  float cos_thetas[3] = {0};
  for (int i = 0; i < 3; i++) {
    V3f light_dir = v3f_norm(v3f_sub(v3f(0, 10, -10), rd.v[i].p), 1e-6, v3ff(0));
    cos_thetas[i] = fmaxf(0, v3f_dot(rd.v[i].n, light_dir));
  } 
  
  for (int i = rd.min_y; i < rd.max_y; i++) {
    for (int j = rd.min_x; j < rd.max_x; j++) { 
      int u, v, w;
      float z;
      if (!RasterPoint(gc, &rd, i, j, &u, &v, &w, &z)) continue;
      LambertFunction(gc, &rd, i, j, u, v, w, z, cos_thetas);
    }
  }
}

/*
void TriangleFill(GrContext *gc, Vertex v1, Vertex v2, Vertex v3)
{
  v1.p = ScreenSpace(gc, v1.p);
  v2.p = ScreenSpace(gc, v2.p);
  v3.p = ScreenSpace(gc, v3.p);

  int width = gc->framebuffer->width;
  int height = gc->framebuffer->height;

  V3f scale = v3f(width, height, 1);
  V3i v1p = v3i3f(v3f_mul(v1.p, scale));
  V3i v2p = v3i3f(v3f_mul(v2.p, scale));
  V3i v3p = v3i3f(v3f_mul(v3.p, scale));
  
  V3i edge0 = v3i_sub(v2p, v1p);
  V3i edge1 = v3i_sub(v3p, v2p);
  V3i edge2 = v3i_sub(v1p, v3p);
  
  int det = (edge0.x * edge1.y) - (edge0.y * edge1.x);
  if (det <= 0) return;
  
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

  RasterData rd = {0};
  if (!VertexStage(gc, &rd)) return;
  
  for (int i = rd.min_y; i < rd.max_y; i++) {
    for (int j = rd.min_x; j < rd.max_x; j++) { 
      int x = j;
      int y = (height - i) - 1;
      int u = (edge1.x * (y-v2p.y)) - (edge1.y * (x-v2p.x));
      int v = (edge2.x * (y-v3p.y)) - (edge2.y * (x-v3p.x));
      int w = (edge0.x * (y-v1p.y)) - (edge0.y * (x-v1p.x));

      bool inside = true;
      inside &= u == 0 ? ((edge1.x > 0 && edge1.y == 0) || edge1.y < 0) : u > 0;
      inside &= v == 0 ? ((edge2.x > 0 && edge2.y == 0) || edge2.y < 0) : v > 0;
      inside &= w == 0 ? ((edge0.x > 0 && edge0.y == 0) || edge0.y < 0) : w > 0;

      //if (debug_draw) gc->framebuffer->buf[j + (i * gc->framebuffer->width)] = (Color) {255, 0, 255, 255};
      if (!inside) continue;
      
      float z = 1.0f / ((((1.0f/v1.p.z)*u)/det) + (((1.0f/v2.p.z)*v)/det) + (((1.0f/v3.p.z)*w)/det));
      unsigned int z_int = (unsigned int)((z / 100.0f) * (float)0xFFFFFFFF);
      unsigned int z_buffer_z = gc->depth_buffer->buf[j + (i * gc->depth_buffer->width)];
      if (z_int > z_buffer_z) continue;
      gc->depth_buffer->buf[j + (i * gc->depth_buffer->width)] = z_int;
      
      V2f tc = v2f(
        z * (((v1.tc.x*u)/(v1.p.z))/det + ((v2.tc.x*v)/(v2.p.z))/det + ((v3.tc.x*w)/(v3.p.z))/det),
        z * (((v1.tc.y*u)/(v1.p.z))/det + ((v2.tc.y*v)/(v2.p.z))/det + ((v3.tc.y*w)/(v3.p.z))/det)
      );

      Color tex_color = SampleImage(gc->sampled_texture, tc);
      V3f tex_vec = v3f_div(v3f(rgamma_lut[tex_color.r], rgamma_lut[tex_color.g], rgamma_lut[tex_color.b]), v3ff(255));

      V3f ambient = v3f(0.5, 0.5, 0.5);

      V3f final = v3f_mul(tex_vec, ambient);

      final = v3f_clamp(final, v3ff(0), v3ff(1));
      final = v3f_mul(final, v3ff(255));
      
      final.x = gamma_lut[(int)round(final.x)];
      final.y = gamma_lut[(int)round(final.y)];
      final.z = gamma_lut[(int)round(final.z)];
      
      Color color = (Color) {V3f_Arg(final), 255};
      gc->framebuffer->buf[j + (i * gc->framebuffer->width)] = color;
    }
  }
}

void LambertTriangleFill(GrContext *gc, Vertex v1, Vertex v2, Vertex v3)
{
  int width = gc->framebuffer->width;
  int height = gc->framebuffer->height;

  V3f scale = v3f(width, height, 1);
  V3i v1p = v3i3f(v3f_mul(v1.p, scale));
  V3i v2p = v3i3f(v3f_mul(v2.p, scale));
  V3i v3p = v3i3f(v3f_mul(v3.p, scale));
  
  V3i edge0 = v3i_sub(v2p, v1p);
  V3i edge1 = v3i_sub(v3p, v2p);
  V3i edge2 = v3i_sub(v1p, v3p);
  
  int det = (edge0.x * edge1.y) - (edge0.y * edge1.x);
  if (det <= 0) return;
  
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

  float cos_thetas[3] = {0};
  Vertex verts[3] = {v1, v2, v3};
  
  for (int i = 0; i < 3; i++) {
    V3f light_dir = v3f_norm(v3f_sub(v3f(0, 10, -10), verts[i].p), 1e-6, v3ff(0));
    cos_thetas[i] = fmaxf(0, v3f_dot(verts[i].n, light_dir));
  } 
  
  for (int i = min_y; i < max_y; i++) {
    for (int j = min_x; j < max_x; j++) { 
      int x = j;
      int y = (height - i) - 1;
      int u = (edge1.x * (y-v2p.y)) - (edge1.y * (x-v2p.x));
      int v = (edge2.x * (y-v3p.y)) - (edge2.y * (x-v3p.x));
      int w = (edge0.x * (y-v1p.y)) - (edge0.y * (x-v1p.x));

      bool inside = true;
      inside &= u == 0 ? ((edge1.x > 0 && edge1.y == 0) || edge1.y < 0) : u > 0;
      inside &= v == 0 ? ((edge2.x > 0 && edge2.y == 0) || edge2.y < 0) : v > 0;
      inside &= w == 0 ? ((edge0.x > 0 && edge0.y == 0) || edge0.y < 0) : w > 0;

      //if (debug_draw) gc->framebuffer->buf[j + (i * gc->framebuffer->width)] = (Color) {255, 0, 255, 255};
      if (!inside) continue;
      
      float z = 1.0f / ((((1.0f/v1.p.z)*u)/det) + (((1.0f/v2.p.z)*v)/det) + (((1.0f/v3.p.z)*w)/det));
      unsigned int z_int = (unsigned int)((z / 100.0f) * (float)0xFFFFFFFF);
      unsigned int z_buffer_z = gc->depth_buffer->buf[j + (i * gc->depth_buffer->width)];
      if (z_int != z_buffer_z) continue;
      
      V2f tc = v2f(
        z * (((v1.tc.x*u)/(v1.p.z))/det + ((v2.tc.x*v)/(v2.p.z))/det + ((v3.tc.x*w)/(v3.p.z))/det),
        z * (((v1.tc.y*u)/(v1.p.z))/det + ((v2.tc.y*v)/(v2.p.z))/det + ((v3.tc.y*w)/(v3.p.z))/det)
      );
      
      float cos_theta_int = z * (((cos_thetas[0]*u)/v1.p.z)/det + ((cos_thetas[1]*v)/v2.p.z)/det + ((cos_thetas[2]*w)/v3.p.z)/det);
      
      Color tex_color = SampleImage(gc->sampled_texture, tc);
      V3f tex_vec = v3f_div(v3f(rgamma_lut[tex_color.r], rgamma_lut[tex_color.g], rgamma_lut[tex_color.b]), v3ff(255));
      
      Color fb_color = gc->framebuffer->buf[j + (i * gc->framebuffer->width)];
      V3f fb_vec = v3f_div(v3f(rgamma_lut[fb_color.r], rgamma_lut[fb_color.g], rgamma_lut[fb_color.b]), v3ff(255));
      
      V3f final = v3f_add(v3f_mul(tex_vec, v3ff(cos_theta_int)), fb_vec);
      
      final = v3f_clamp(final, v3ff(0), v3ff(1));      
      final = v3f_mul(final, v3ff(255));

      final.x = gamma_lut[(int)round(final.x)];
      final.y = gamma_lut[(int)round(final.y)];
      final.z = gamma_lut[(int)round(final.z)];
      
      Color color = (Color) {V3f_Arg(final), 255};
      gc->framebuffer->buf[j + (i * gc->framebuffer->width)] = color;
    }
  }
}
*/
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
  
  switch (gc->mode) {
  case RENDER_MODE_LAMBERT:
    for (size_t i = 0; i < gc->verts.count; i += 3) {
      LambertTriangleFill(gc, gc->verts.items[i], gc->verts.items[i+1], gc->verts.items[i+2]);
    }
    break;
  case RENDER_MODE_FLAT_AMBIENT:
    for (size_t i = 0; i < gc->verts.count; i += 3) {
      AmbientTriangleFill(gc, gc->verts.items[i], gc->verts.items[i+1], gc->verts.items[i+2]);
    }
    break;
  };
  
  gc->verts.count = 0;
}
