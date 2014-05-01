#include "raytracer.h"

double double_max(double a, double b)
{
  return a>b ? a : b;
}

hit_test intersect(ray r, sphere s)
{
  hit_test result = {0}; /* {0} to quiet the compiler */
  vec sc = s.center;
  double sr = s.radius;
  vec A = vec_sub(r.origin, sc);
  double B = vec_dot(A, r.direction);
  double C = vec_dot(A, A) - (sr * sr);
  double D = (B * B) - C;
  double t = (-B - sqrt(D));
  result.miss = 1;
  if (D > 0 && t > 0) {
    result.miss = 0;
    result.dist = t;
    result.surf = s.surf;
    result.shine = s.shine;
    result.surf_norm = vec_norm(vec_sub(ray_position(r,t),sc));
  }
  return result;
}

int shadow(vec pt, light l, sphere_list *ss)
{
  vec nudge = vec_scale(0.0001, l.direction);
  vec lifted = vec_add(pt, nudge);
  ray test_ray = {lifted, l.direction};
  while (ss!=NULL) {
    hit_test h = intersect(test_ray, ss->s);
    if (!h.miss)
      return 1;
    ss = ss->next;
  }
  return 0;
}

int rgb_nonzero(rgb c)
{
  return (c.r>0 || c.g>0 || c.b>0);
}

rgb lighting(scene s, ray r, hit_test h)
{
  rgb result;
  if (h.miss)
    return s.bg;
  vec hit_position = ray_position(r, h.dist);
  if (shadow(hit_position, s.light, s.spheres)) {
    result = rgb_modulate(h.surf, s.amb);
  }
  else {  
    double dot = vec_dot(h.surf_norm, s.light.direction);
    double d = double_max(0, dot);
    rgb diffuse_light = rgb_scale(d, s.light.color);
    rgb lsum = rgb_add(s.amb, diffuse_light);
    result = rgb_modulate(h.surf, lsum);
  }
  /**** === implement specular reflection here === ****/
 
  if (rgb_nonzero(h.shine)) {
    rgb ss;
    vec N = h.surf_norm;
    vec L = s.light.direction;
    rgb S = h.shine;
    vec R = vec_sub( vec_scale(2* vec_dot(N,L),N),L);
    vec V = vec_neg(r.direction);
    if (vec_dot(N,L)>0){
      ss = rgb_scale( pow( double_max( vec_dot(R,V),0), 6), S);
      //rgb_print(k);
    }
    else
      ss = rgb_expr(0,0,0);
    return rgb_add(result,ss);
  }
  return result;
}

vec logical_loc(camera c, vec p)
{
  double pixw, pixh;
  double lx, ly;
  if (c.h>c.w)
    pixh = pixw = 2.0 / c.h;
  else
    pixh = pixw = 2.0 / c.w;
  if (p.x>c.w || p.y>c.h) {
    fprintf(stderr, "logical_loc: bad position (%lg, %lg)\n", p.x, p.y);
    exit(1);
  }
  lx = -1.0 + (pixw * p.x) + (pixw / 2.0);
  ly =  1.0 - ((pixh * p.y) + (pixh / 2.0));
  if (c.w>c.h)
    ly -= (c.w-c.h) / 2.0 * pixh;
  else
    lx += (c.h-c.w) / 2.0 * pixw;
  vec result = {lx, ly, 0};
  return result;
}

rgb trace_ray(scene s, ray r)
{
  hit_test closest_hit = {0}, /* {0} to quiet the compiler */
           temp_hit = {0};
  sphere_list *ss = s.spheres;
  closest_hit.miss=1;
  while (ss!=NULL) {
    temp_hit = intersect(r, ss->s);
    if (closest_hit.miss)
      closest_hit = temp_hit;
    else if (!(temp_hit.miss) && !(closest_hit.miss) &&
	     temp_hit.dist < closest_hit.dist)
      closest_hit = temp_hit;
    ss = ss->next;
  }
  return lighting(s, r, closest_hit);
}

void render(stage g)
{
  int i, j;
  camera c = g.c;
  scene sc = g.s;
  printf("P3\n");
  printf("%d %d\n", c.w, c.h);
  printf("255\n");
  for(i=0; i < c.h; i++) {
    for(j=0; j < c.w; j++) {
      vec p = {j,i,0}; 
      vec loc = logical_loc(c, p);
      vec dir = vec_sub(loc, c.loc);
      vec normdir = vec_norm(dir);
      ray r = {c.loc, normdir};
      rgb col = trace_ray(sc, r);
      rgb_print_bytes(col);
    }
  }  
}

/* ===== Project 2 Code ===== */



/* fintersect is like intersect, but uses the timestamp */
/* to compute the surface color and specular reflection */
/* color of the intersecting fsphere (if there is one)  */
hit_test fintersect(ray r, fsphere s, timestamp ts)
{
  hit_test result = {0}; /* {0} to quiet the compiler */
  vec sc = s.center;
  double sr = s.radius;
  vec A = vec_sub(r.origin, sc);
  double B = vec_dot(A, r.direction);
  double C = vec_dot(A, A) - (sr * sr);
  double D = (B * B) - C;
  double t = (-B - sqrt(D));
  result.miss = 1;
  if (D > 0 && t > 0) {
    result.miss = 0;
    result.dist = t;
    result.surf = s.compute_surf(s.center, ray_position(r,t), ts);
    result.shine = s.compute_shine(ts);
    result.surf_norm = vec_norm(vec_sub(ray_position(r,t),sc));
  }
  return result;
}

/* fshadow is like shadow, but needs to pass a timestamp */
/* to fintersect                                         */
int fshadow(vec p, light l, fsphere_list *ss, timestamp ts)
{
  vec nudge = vec_scale(0.0001, l.direction);
  vec lifted = vec_add(p, nudge);
  ray test_ray = {lifted, l.direction};
  while (ss!=NULL) {
    hit_test h = fintersect(test_ray, ss->s, ts);
    if (!h.miss)
      return 1;
    ss = ss->next;
  }
  return 0;
}

/* flighting is like lighting, but consumes a frame */
/* rather than a scene                              */
rgb flighting(frame f, ray r, hit_test h)
{
  rgb result;
  if (h.miss)
    return f.bg;
  vec hit_position = ray_position(r, h.dist);
  if (fshadow(hit_position, f.light, f.spheres, f.ts)) {
    result = rgb_modulate(h.surf, f.amb);
  }
  else {  
    double dot = vec_dot(h.surf_norm, f.light.direction);
    double d = double_max(0, dot);
    rgb diffuse_light = rgb_scale(d, f.light.color);
    rgb lsum = rgb_add(f.amb, diffuse_light);
    result = rgb_modulate(h.surf, lsum);
  }
  /**** === implement specular reflection here === ****/
 
  if (rgb_nonzero(h.shine)) {
    rgb ss;
    vec N = h.surf_norm;
    vec L = f.light.direction;
    rgb S = h.shine;
    vec R = vec_sub( vec_scale(2* vec_dot(N,L),N),L);
    vec V = vec_neg(r.direction);
    if (vec_dot(N,L)>0){
      ss = rgb_scale( pow( double_max( vec_dot(R,V),0), 6), S);
      //rgb_print(k);
    }
    else
      ss = rgb_expr(0,0,0);
    return rgb_add(result,ss);
  }
  return result;
}

/* ftrace is like trace, but for frames rather than scenes */
rgb ftrace(ray r, frame f)
//rgb trace_ray(scene s, ray r)
{
  hit_test closest_hit = {0}, /* {0} to quiet the compiler */
           temp_hit = {0};
  fsphere_list *ss = f.spheres;
  closest_hit.miss=1;
  while (ss!=NULL) {
    temp_hit = fintersect(r, ss->s, f.ts);
    if (closest_hit.miss)
      closest_hit = temp_hit;
    else if (!(temp_hit.miss) && !(closest_hit.miss) &&
	     temp_hit.dist < closest_hit.dist)
      closest_hit = temp_hit;
    ss = ss->next;
  }
  return flighting(f, r, closest_hit);
}

/* frender consumes a camera and a frame and renders the frame, */
/* writing a ppm file to the given filename                     */
/* - use fopen to open the file, and fclose when you're done    */
void frender(camera c, frame f, char *filename)
//void render(stage g)
{
  char name[32];
  int i, j;
  for (i=0; i<32; i++)
    name[i]='\0';
  sprintf(name, "%s.ppm", filename);
  FILE *fp = fopen (name, "a");
  fprintf(fp, "P3\n");
  fprintf(fp, "%d %d\n", c.w, c.h);
  fprintf(fp, "255\n");
  for(i=0; i < c.h; i++) {
    for(j=0; j < c.w; j++) {
      vec p = {j,i,0}; 
      vec loc = logical_loc(c, p);
      vec dir = vec_sub(loc, c.loc);
      vec normdir = vec_norm(dir);
      ray r = {c.loc, normdir};
      rgb col = ftrace(r, f);
      //rgb_print_bytes(col);
      fprintf(fp, "%d %d %d\n",bytify(col.r),bytify(col.g),bytify(col.b));
    }
  }  
  fclose(fp);
}

void frender_frames(fstage g, char *name_stem)
{
  unsigned int frames = g.n_frames;
  int i;  
  char tmp[16];
  for (i=0; i<16; i++)
    tmp[i]='\0';
  for (i=0; i<frames; i++){
    sprintf(tmp, "%s%06d", name_stem, i);
    frender(g.c, g.compute_frame(i), tmp);
  }
}
      
    



int main(int argc, char *argv[])
{
  fstage g = get_fstage(NULL);
  char name[16];
  int i;
  for (i=0; i<16; i++)
    name[i]='\0';
  sprintf(name, "Hithere");
  frender_frames(g, name);
  return 0;
}
