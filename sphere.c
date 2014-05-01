#include "raytracer.h"

sphere sphere_expr(vec v, double r, rgb surf, rgb shine)
{
  sphere x = {v,r,surf,shine};
  return x;
}

char *sphere_tos(sphere s)
{
  char buf[1024] = {0};
  char *s1 = vec_tos(s.center);
  char *s2 = rgb_tos(s.surf);
  char *s3 = rgb_tos(s.shine); 
  sprintf(buf,"sphere(%s,%lf,%s,%s)",s1,s.radius,s2,s3);
  free(s1);
  free(s2);
  free(s3);
  return strdup(buf);
}

void sphere_print(sphere s)
{
  char *str = sphere_tos(s);
  printf("%s\n",str);
  free(str);
}
