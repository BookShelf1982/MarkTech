#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

#define CFLAGS "-Wall", "-Wextra"

Cmd cmd = {0};

int main(int argc, char **argv)
{
  NOB_GO_REBUILD_URSELF(argc, argv);

  if (!mkdir_if_not_exists("bin")) return 1;

  cmd_append(&cmd, "cc", CFLAGS, "-o", "bin/lutgen", "lutgen.c");
  if (!cmd_run(&cmd)) return 1;

  cmd_append(&cmd, "./bin/lutgen");
  if (!cmd_run(&cmd)) return 1;
  
  cmd_append(&cmd, "cc", CFLAGS, "-O2", "-o", "bin/main",
  "main.c", "graphics.c", "osx_window.m",
  "-framework", "AppKit",
  "-framework", "QuartzCore",
  "-framework", "AVFoundation");
  
  if (!cmd_run(&cmd)) return 1;
  
  return 0;
}
