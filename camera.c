#include "raytracer.h"

char *camera_tos(camera c)
{
  char buf[1024] = {0};
  char *s = vec_tos(c.loc);
  sprintf(buf,"camera(%s,%d,%d)",s,c.h,c.w);
  free(s);
  return strdup(buf);
}

void camera_print(camera c)
{
  char *s = camera_tos(c);
  printf("%s\n",s);
  free(s);
}
