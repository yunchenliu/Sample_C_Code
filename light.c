#include "raytracer.h"

char *light_tos(light l)
{
  char buf[1024] = {0};
  char *s1 = vec_tos(l.direction);
  char *s2 = rgb_tos(l.color);
  sprintf(buf,"light(%s,%s)",s1,s2);
  free(s1);
  free(s2);
  return strdup(buf);
}

void light_print(light l)
{
  char *s = light_tos(l);
  printf("%s\n",s);
  free(s);
}
