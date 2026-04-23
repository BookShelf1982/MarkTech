#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

Cmd cmd = {0};

int main(int argc, char **argv)
{
  NOB_GO_REBUILD_URSELF(argc, argv);

  if (!mkdir_if_not_exists("bin")) return 1;
  
  cmd_append(&cmd, "cc", "-O3", "-Wall", "-Wextra", "-o", "bin/main",
  "main.c", "graphics.c", "osx_window.m",
  "-framework", "AppKit",
  "-framework", "QuartzCore");
  
  if (!cmd_run(&cmd)) return 1;
  return 0;
}
