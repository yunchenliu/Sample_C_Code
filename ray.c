#include "raytracer.h"

vec ray_position(ray r, double t)
{
  return vec_add(r.origin,vec_scale(t,r.direction));
}

char *ray_tos(ray r)
{
  char buf[1024] = {0};
  char *s1 = vec_tos(r.origin);
  char *s2 = vec_tos(r.direction);
  sprintf(buf,"ray(o:%s,dir:%s)",s1,s2);
  free(s1);
  free(s2);
  return strdup(buf);
}
