#include "raytracer.h"

rgb rgb_expr(double r, double g, double b)
{
  rgb c = {r,g,b};
  return c;
}

double rgb_clamp_above(double x)
{
  return (x>1)?1:x;
}

double rgb_clamp(double x)
{
  return (x<0)?0:((x>1)?1:x);
}

rgb rgb_modulate(rgb x, rgb y)
{
  rgb res = {x.r*y.r, x.g*y.g, x.b*y.b};
  return res;
}

rgb rgb_scale(double s, rgb c)
{
  rgb res = {rgb_clamp(s*c.r), 
	     rgb_clamp(s*c.g), 
	     rgb_clamp(s*c.b)}; 
  return res;
}

rgb rgb_add(rgb x, rgb y)
{
  rgb res = {rgb_clamp_above(x.r+y.r), 
	     rgb_clamp_above(x.g+y.g), 
	     rgb_clamp_above(x.b+y.b)};
  return res;
}

void rgb_print(rgb x)
{
  printf("(%lg, %lg, %lg)\n", x.r, x.g, x.b);
}

char *rgb_tos(rgb x)
{
  char buf[1024] = {0};
  sprintf(buf,"RGB(%lf,%lf,%lf)",x.r,x.g,x.b);
  return strdup(buf);
}

byte bytify(double x)
{
  return (byte)(x*255);
}

void rgb_print_bytes(rgb c)
{
  printf("%d %d %d\n",bytify(c.r),bytify(c.g),bytify(c.b));
}
