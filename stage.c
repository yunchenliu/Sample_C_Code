#include "raytracer.h"

stage get_stage(char *fut)
{
  camera test_camera_1 = {{0,0,-5},288,512}; 
  /* n.b. 16x9 is the cinematic widescreen ratio :-) */
  sphere sph0 = {{0.67,0,4},0.67,{0.4,0.6,1.0},{1,1,1}};
  sphere_list *spheres = sl_singleton(sph0);
  vec ldir = vec_norm(vec_expr(-3,1,-1));
  light lt = {ldir,{1,1,1}};
  scene test_scene_1 = {{0.1,0.1,0.4},spheres,lt,{0.33,0.33,0.33}};
  stage g = {test_camera_1, test_scene_1};
  return g;
}

/* rgb col_function(vec center, vec loc, timestamp t) */
/* { */
/*     rgb new= rgb_expr(sin(loc.x*t), sin(loc.y*t), sin(loc.z*t)); */
/*     return new; */
/* } */

/* rgb shinef(timestamp t) */
/* { */
/*     rgb new= rgb_expr(.4, .4, .4); */
/*     return new; */
/* } */

/* frame f_function(timestamp ts) */
/* { */
/*     vec center= vec_expr(.67, 0, 4); */
/*     double radius= .67; */
/*     fsphere fs= {center, radius, &col_function, &shinef}; */
    
/*     vec ldir = vec_norm(vec_expr(-3,1,-1)); */
/*     light lt = {ldir,{1,1,1}}; */
    
/*     frame f; */
/*     f.ts= ts; */
/*     f.bg= rgb_expr(.1, .1, .4); */
/*     f.spheres= fsl_singleton(fs); */
/*     f.light= lt; */
/*     f.amb= rgb_expr(.33, .33, .33); */
    
/*     return f; */
/* } */


rgb col_function(vec center, vec loc, timestamp t)
{
 if ((sin(50*loc.y*(t+1))) > 0.5) {
 return rgb_expr(0.7, 0.7, 0.7);
 }
 else {
 return rgb_expr(1, 1, 1);
 }
}

rgb shinef(timestamp t)
{
 rgb new= rgb_expr(t, t, t);
 return new;
}

rgb col_function2(vec center, vec loc, timestamp t)
{
 if ((sin(50*loc.y*(t+1))) > 0.5) {
 return rgb_expr(0.9, 0.1, 0.1);
 }
 else {
 return rgb_expr(1, 1, 1);
 }
}

rgb shinef2(timestamp t)
{
 rgb new= rgb_expr(1, 1, 1);
 return new;
}

frame f_function(timestamp ts)
{
 vec center = vec_expr(0.7*sin(1.7+0.3*ts), 0.5, 4+4*cos(0.3*ts+1.7));
 vec center2 = vec_expr(0.3*sin(0.3*ts), 0.5, 4+2*cos(0.3*ts));
 double radius = .3;
 fsphere fs= {center, radius, &col_function, &shinef};
 fsphere fs2 = {center2, radius, &col_function2, &shinef2};
 
 vec ldir = vec_norm(vec_expr(-3,1,-1));
 light lt = {ldir,{1,1,1}};
 
 frame f;
 f.ts= ts;
 f.bg= rgb_expr(.1, .1, .4);
 f.spheres= fsl_singleton(fs);
 f.spheres= fsl_cons(fs2, f.spheres);
 f.light= lt;
 f.amb= rgb_expr(.33, .33, .33);
 
 return f;
}

fstage get_fstage(char *fut)
{
 camera test_camera_1 = {{0,0,-5}, 500, 500};
 timestamp ts= 3;
 fstage fs= {test_camera_1, &f_function, ts};
 
 return fs;
}

/* fstage get_fstage(char *fut) */
/* { */
/*     camera test_camera_1 = {{0,0,-5},288,512}; */
/*     timestamp ts= 4; */
/*     fstage fs= {test_camera_1, &f_function, ts}; */
 
/*  return fs; */
/* } */
