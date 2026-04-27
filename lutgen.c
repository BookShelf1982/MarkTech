#include <stdio.h>
#include <math.h>

#define SAMPLES 256
#define GAMMA 2.2f
#define DEFAULT_FILEPATH "gamma_lut.c"

int main(int argc, const char **argv)
{
  const char *filepath = argc > 1 ? argv[1] : DEFAULT_FILEPATH;
  FILE *f = fopen(filepath, "wb");
  if (!f) return 1;

  fprintf(f, "#define GAMMA_LUT_LEN %d\n\n", SAMPLES);
  
  fprintf(f, "//Gamma encode\n\n");
  fprintf(f, "static float gamma_lut[] = {\n  ");
  for (int i = 0; i < SAMPLES; i++) {
    float a = (float)i / (float)SAMPLES;
    float b = powf(a, GAMMA) * 255;
    fprintf(f, "%f", b);
    if (i + 1 <= SAMPLES) {
      fprintf(f, ", ");
      if ((i + 1) % 4 == 0) fprintf(f, "\n  ");
    }
  }
  fprintf(f, "\n};\n");
  
  fprintf(f, "//Gamma decode\n\n");
  fprintf(f, "static float rgamma_lut[] = {\n  ");
  for (int i = 0; i < SAMPLES; i++) {
    float a = (float)i / (float)SAMPLES;
    float b = powf(a, 1/GAMMA) * 255;
    fprintf(f, "%f", b);
    if (i + 1 <= SAMPLES) {
      fprintf(f, ", ");
      if ((i + 1) % 4 == 0) fprintf(f, "\n  ");
    }
  }
  fprintf(f, "\n};\n");
  
  return 0;
}


