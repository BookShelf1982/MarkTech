#include <stdio.h>

#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "xdmath.h"
#include "graphics.h"
#include "osx_window.h"

#define SCALE 40
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

int main(int argc, const char **argv)
{
  if (argc < 2) return 1;
  InitFramebuffer(WIDTH, HEIGHT);
  Image texture = LoadImage(argv[1]);
  if (!texture.buf) {
    fprintf(stderr, "Failed to find file: %s\n", argv[1]);
    return 2;
  }
  gc.sampled_texture = &texture;
  
  CreateAndRunNSApp(fb.buf, WIDTH, HEIGHT, &DrawIt);
  
  FreeFramebuffer();
  stbi_image_free(texture.buf);

  printf("ded.\n");
  return 0;
}
