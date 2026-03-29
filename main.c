#include <stdio.h>
#include <float.h>

#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "graphics.h"
#include "osx_window.h"
#include "scancodes.h"

typedef struct {
  V3f pos;
  V3f forward;
  float pitch, yaw;
} Camera;

M4f ViewMatrix(Camera *c)
{
  M4f t = m4f_id();
  t._14 = -c->pos.x;
  t._24 = -c->pos.y;
  t._34 = -c->pos.z;

  M4f r = m4f_mul(m4f_rot_x(c->pitch * (M_PI / 180)), m4f_rot_y(-c->yaw * (M_PI / 180)));

  return m4f_mul(r, t);
}

void UpdateCamera(Camera *c, float dt)
{
  float wish_x = IsKeyDown(SCANCODE_D) ? 1 : IsKeyDown(SCANCODE_A) ? -1 : 0;
  float wish_y = IsKeyDown(SCANCODE_W) ? 1 : IsKeyDown(SCANCODE_S) ? -1 : 0;

  float wish_yaw = IsKeyDown(SCANCODE_RIGHT) ? 1 : IsKeyDown(SCANCODE_LEFT) ? -1 : 0;
  float wish_pitch = IsKeyDown(SCANCODE_UP) ? 1 : IsKeyDown(SCANCODE_DOWN) ? -1 : 0;
  c->yaw = fmodf(c->yaw + 180 * wish_yaw * dt, 360);
  c->pitch = fmodf(c->pitch + 180 * wish_pitch * dt, 360);

  float yaw_radians = c->yaw * (M_PI / 180);
  float pitch_radians = c->pitch * (M_PI / 180);
  float cos_yaw = cosf(yaw_radians);
  float sin_yaw = sinf(yaw_radians);
  float cos_pitch = cosf(pitch_radians);
  float sin_pitch = sinf(pitch_radians);
  V3f forward = v3f(cos_pitch*sin_yaw, sin_pitch, cos_yaw*cos_pitch);
  c->forward = forward;
  V3f right = v3f_cross(v3f(0, 1, 0), forward);

  float speed = 5;
  V3f move_dir = v3f_add(v3f_mul(forward, v3ff(wish_y)), v3f_mul(right, v3ff(wish_x)));
  V3f dp = v3f_mul(v3f_norm(move_dir, FLT_EPSILON, v3ff(0)), v3ff(speed * dt));
  c->pos = v3f_add(c->pos, dp);
}

#define SCALE 160

#define WIDTH 4*SCALE
#define HEIGHT 3*SCALE

Framebuffer fb = {0};
DepthBuffer db = {0};
GrContext gc = {0};

void InitFramebuffer(unsigned int width, unsigned int height)
{
  void *ptr = malloc(2 * (width * height * 4));
  fb = (Framebuffer){
    .buf = ptr,
    .width = width,
    .height = height
  };
  db = (DepthBuffer){
    .buf = ((unsigned int *)((Color *)ptr + (width * height))),
    .width = width,
    .height = height
  };
  gc = (GrContext){
    .framebuffer = &fb,
    .depth_buffer = &db
  };
}

void FreeFramebuffer(void)
{
  free(fb.buf);
}

Image LoadImage(const char *filepath)
{
  Image img = {0};
  int channels = 0;
  int desired_channels = 4;
  img.buf = stbi_load(filepath, &img.width, &img.height, &channels, desired_channels);
  return img;
}

typedef struct {
  size_t index_count;
  size_t vertex_count;
  unsigned int *indices;
  Vertex *vertices;
} Model;

Model LoadModel(const char *filepath)
{
  FILE *f = fopen(filepath, "rb");
  if (!f) {
    fprintf(stderr, "Failed to find file: %s\n", filepath);
    return (Model){0};
  }
  size_t vert_count = 0;
  size_t index_count = 0;
  fread(&index_count, sizeof(index_count), 1, f);
  fread(&vert_count, sizeof(vert_count), 1, f);
  size_t required_size = (4 * index_count) + (sizeof(Vertex) * vert_count);
  void *ptr = malloc(required_size);
  fread(ptr, required_size, 1, f);
  return (Model) {
    .index_count = index_count,
    .vertex_count = vert_count,
    .indices = ptr,
    .vertices = (Vertex *)((char *)ptr + (4 * index_count))
  };
}

Model test_model = {0};
Camera camera = {0};

float theta = 0.0f;
float speed = (M_PI / 2.0);
V3f translate = (V3f) {0, 0, 5};
float scale = 1.0f;
float aspect;

void DrawIt(void)
{
  GrClear(&gc);
  
  M4f translate_matrix = m4f_id();
  translate_matrix._14 = translate.x;
  translate_matrix._24 = translate.y;
  translate_matrix._34 = translate.z;

  M4f transform = m4f_mul(translate_matrix, m4f_rot_y(theta));
  
  gc.transform = m4f_mul(ViewMatrix(&camera), transform);
  
  for (size_t i = 0; i < test_model.index_count; i += 3) {
    Vertex v1 = test_model.vertices[test_model.indices[i]];
    Vertex v2 = test_model.vertices[test_model.indices[i + 1]];
    Vertex v3 = test_model.vertices[test_model.indices[i + 2]];
    v1.p = ScreenSpace(&gc, m4f_mul_vec(gc.transform, v4f(V3f_Arg(v1.p), 1)).xyz);
    v2.p = ScreenSpace(&gc, m4f_mul_vec(gc.transform, v4f(V3f_Arg(v2.p), 1)).xyz);
    v3.p = ScreenSpace(&gc, m4f_mul_vec(gc.transform, v4f(V3f_Arg(v3.p), 1)).xyz);
    GrTriangle(&gc, v1, v2, v3);
  }
}

int main(int argc, const char **argv)
{
  if (argc < 3) return 1;
  InitFramebuffer(WIDTH, HEIGHT);
  Image texture = LoadImage(argv[1]);
  test_model = LoadModel(argv[2]);
  if (!texture.buf || !test_model.vertices) {
    fprintf(stderr, "Failed\n");
    return 2;
  }
  gc.sampled_texture = &texture;
  
  InitWindow(fb.buf, WIDTH, HEIGHT, &DrawIt);
  uint64_t prev_time = 0;
  while (!ShouldWindowClose()) {
    uint64_t now = GetHighResTime();
    uint64_t elapsed = now - prev_time;
    float dt = elapsed * 1e-9;
    prev_time = now;
    
    ProcessWindowEvents();
    if (IsKeyPressed(SCANCODE_ESCAPE)) break;
    UpdateCamera(&camera, dt);
    theta = fmodf(theta + (M_PI * 0.5) * dt, M_PI * 2);
  }

  CloseWindow();
  FreeFramebuffer();
  free(test_model.indices);
  stbi_image_free(texture.buf);

  printf("ded.\n");
  return 0;
}
