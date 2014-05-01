#include "raytracer.h"

vec vec_expr(double x, double y, double z)
{
  vec v = {x,y,z};
  return v;
}

vec vec_add(vec v1, vec v2)
{
  vec r = {v1.x+v2.x, v1.y+v2.y, v1.z+v2.z};
  return r;
}

vec vec_sub(vec v1, vec v2)
{
  vec r = {v1.x-v2.x, v1.y-v2.y, v1.z-v2.z};
  return r;
}

vec vec_scale(double s, vec v)
{
  vec r = {s*v.x, s*v.y, s*v.z};
  return r;
}

vec vec_neg(vec v)
{
  return vec_scale(-1, v);
}

double vec_dot(vec v1, vec v2)
{
  return (v1.x*v2.x)+(v1.y*v2.y)+(v1.z*v2.z);
}

double vec_mag(vec v)
{
  return sqrt(vec_dot(v, v));
}

vec vec_norm(vec v)
{
  double m = vec_mag(v);
  return fabs(m)<0.0001?v:vec_scale(1.0/m,v);
}

void vec_print(vec v)
{
  printf("vec(%lg,%lg,%lg)\n",v.x,v.y,v.z);
}

char *vec_tos(vec v)
{
  char buf[1024] = {0};
  sprintf(buf,"vec(%lf,%lf,%lf)",v.x,v.y,v.z);
  return strdup(buf);
}

