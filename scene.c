#include "raytracer.h"

char *scene_tos(scene s)
{
  fprintf(stderr,"TODO: scene_tos\n");
  exit(1);
}

void scene_print(scene s) 
{
  printf("*** BEGIN scene\n");
  rgb_print(s.bg);
  sl_print(s.spheres);
  light_print(s.light);
  rgb_print(s.amb);
  printf("*** END scene\n");
}

