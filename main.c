#include <stdio.h>

#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define XDMATH_IMPLEMENTATION
#include "xdmath.h"

#include "graphics.h"
#include "osx_window.h"

#define SCALE 20

#define WIDTH 16*SCALE
#define HEIGHT 9*SCALE

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
    .buf = ((unsigned int *)ptr + (width * height)),
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

float theta = 0.0f;
D3 translate = {0.0, 0.0f, 5.0f};
float aspect;

void DrawIt(float dt)
{
  GrClear(&gc);

  gc.transform = M4x4Fuck(M4x4RotateYAxis(theta), M4x4Translate(translate.x, translate.y, translate.z));
  
  for (size_t i = 0; i < test_model.index_count; i += 3) {
    Vertex v1 = test_model.vertices[test_model.indices[i]];
    Vertex v2 = test_model.vertices[test_model.indices[i + 1]];
    Vertex v3 = test_model.vertices[test_model.indices[i + 2]];
    v1.p = ScreenSpace(&gc, TrimD4(M4x4MulD4(gc.transform, MakeD4FromD3(v1.p, 1.0f))));
    v2.p = ScreenSpace(&gc, TrimD4(M4x4MulD4(gc.transform, MakeD4FromD3(v2.p, 1.0f))));
    v3.p = ScreenSpace(&gc, TrimD4(M4x4MulD4(gc.transform, MakeD4FromD3(v3.p, 1.0f))));
    GrTriangle(&gc, v1, v2, v3);
  }

  theta += (M_PI / 2.0) * dt;
  theta = fmod(theta, (M_PI * 2));
}

int main(int argc, const char **argv)
{ 
  if (argc < 3) return 1;
  InitFramebuffer(WIDTH, HEIGHT);
  aspect = WIDTH/HEIGHT;
  Image texture = LoadImage(argv[1]);
  test_model = LoadModel(argv[2]);
  if (!texture.buf || !test_model.vertices) {
    fprintf(stderr, "Failed\n");
    return 2;
  }
  gc.sampled_texture = &texture;
  
  InitWindow(fb.buf, WIDTH, HEIGHT, &DrawIt);
  while (!ShouldWindowClose()) {
    ProcessWindowEvents();
  }

  CloseWindow();
  FreeFramebuffer();
  free(test_model.indices);
  stbi_image_free(texture.buf);

  printf("ded.\n");
  return 0;
}
