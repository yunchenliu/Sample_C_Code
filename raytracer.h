#ifndef __RAYTRACER_H__
#define __RAYTRACER_H__

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

/* ====================================== */ 
/* ====================================== */ 
/* === STRUCTURE AND TYPE DEFINITIONS === */ 
/* ====================================== */ 
/* ====================================== */ 

typedef unsigned char byte;

typedef unsigned int timestamp;

typedef struct {
  double x;
  double y;
  double z;
} vec;

typedef struct {
  /* each value must be on interval [0,1] */
  double r;
  double g;
  double b;
} rgb;

typedef struct {
  vec origin;
  vec direction;
} ray;

typedef struct {
  vec center;
  double radius;
  rgb surf;
  rgb shine;
} sphere;

typedef struct sphere_list sphere_list;
/* convention: NULL is the empty sphere list */
struct sphere_list {
  sphere s;
  sphere_list *next;
};

typedef struct {
  vec direction;
  rgb color;
} light;

typedef struct {
  rgb bg;
  sphere_list *spheres;
  light light;
  rgb amb;
} scene;

typedef struct {
  int miss; /* false (0) for hit, true (nonzero) for miss */
  double dist;
  rgb surf;
  rgb shine;
  vec surf_norm;
} hit_test;

typedef struct {
  vec loc;
  unsigned int h;
  unsigned int w;
} camera;

typedef struct {
  camera c;
  scene s;
} stage;

/* ============================= */ 
/* === PROJECT 2 DEFINITIONS === */ 
/* ============================= */ 

typedef struct {
  vec center;
  double radius;
  rgb(*compute_surf)(vec center, vec loc, timestamp t);
  rgb(*compute_shine)(timestamp t);
} fsphere;

typedef struct fsphere_list fsphere_list;
/* convention: NULL is the empty fsphere list */
struct fsphere_list {
  fsphere s;
  fsphere_list *next;
};

typedef struct {
  timestamp ts;
  rgb bg;
  fsphere_list *spheres;
  light light;
  rgb amb;
} frame;

typedef struct {
  camera c;
  frame(*compute_frame)(timestamp);
  timestamp n_frames;
} fstage;

/* note: an "fstage" is, in other words, a "specification for a film" */
/* or a "specification for a sequence of frames" */

/* ================== */ 
/* ================== */ 
/* === OPERATIONS === */ 
/* ================== */ 
/* ================== */ 

/* Please note: the following is not necessarily every operation you will need,
 * nor will you necessarily use every operation in your implementation.
 * Add and implement additional operations as you require them.
 */

/* ================================== */ 
/* === vector operations => vec.c === */ 
/* ================================== */ 

vec vec_expr(double x, double y, double z);

vec vec_add(vec v1, vec v2);

vec vec_sub(vec v1, vec v2);

vec vec_neg(vec v);

vec vec_scale(double s, vec v);

double vec_dot(vec v1, vec v2);

double vec_mag(vec v);

/* if a vector's magnitude is zero, it can't be normalized; simply return it */
vec vec_norm(vec v);

char *vec_tos(vec v);

void vec_print(vec v);

/* =============================== */ 
/* === rgb operations => rgb.c === */ 
/* =============================== */ 

rgb rgb_expr(double r, double g, double b);

rgb rgb_modulate(rgb c1, rgb c2);

rgb rgb_scale(double s, rgb c);

rgb rgb_add(rgb c1, rgb c2);

char *rgb_tos(rgb c);

void rgb_print(rgb c);

byte bytify(double x);

/* print three integer values on [0,255] with spaces in between and a newline */
void rgb_print_bytes(rgb c);

/* print bytes to FILE pointed to by FILE* f */
void rgb_fprint_bytes(FILE *f, rgb c);

/* =============================== */ 
/* === ray operations => ray.c === */ 
/* =============================== */ 

vec ray_position(ray r, double time);

char *ray_tos(ray r);

void ray_print(ray r);

/* ===================================== */ 
/* === sphere operations => sphere.c === */ 
/* ===================================== */ 

sphere sphere_expr(vec v, double r, rgb c, rgb s);

char *sphere_tos(sphere s);

void sphere_print(sphere s);

/* ======================================== */ 
/* === sphere list ops => sphere_list.c === */ 
/* ======================================== */ 

sphere_list *sl_cons(sphere s, sphere_list *ss);

fsphere_list *fsl_cons(fsphere s, fsphere_list *ss);

sphere_list *sl_singleton(sphere s);

fsphere_list *fsl_singleton(fsphere s);

void sl_print(sphere_list *ss);

void sl_free(sphere_list *ss);

/* =================================== */ 
/* === light operations => light.c === */ 
/* =================================== */ 

char *light_tos(light l);

void light_print(light l);

/* =================================== */ 
/* === scene operations => scene.c === */ 
/* =================================== */ 

char *scene_tos(scene s);

void scene_print(scene s);

/* ========================================= */ 
/* === hit_test operations => hit_test.c === */ 
/* ========================================= */ 

char *hit_test_tos(hit_test t);

void hit_test_print(hit_test t);

/* ===================================== */ 
/* === camera operations => camera.c === */ 
/* ===================================== */ 

char *camera_tos(camera c);

void camera_print(camera c);

/* =================================== */ 
/* === stage operations => stage.c === */ 
/* =================================== */ 

char *stage_tos(stage s);

void stage_print(stage s);

/* The function get_stage just needs to return a valid stage. */
/* The string future_expansion is not used. Maybe some other time. */
stage get_stage(char *future_expansion);

/* ===================================== */ 
/* === fstage operations => fstage.c === */ 
/* ===================================== */ 

/* The function get_fstage just needs to return a valid fstage. */
/* The string future_expansion is not used. */
fstage get_fstage(char *future_expansion);

/* ================================================ */ 
/* ================================================ */ 
/* === RAY TRACER LOGIC and MAIN => raytracer.c === */ 
/* ================================================ */ 
/* ================================================ */ 

/* === project 1 operations === */

hit_test intersect(ray r, sphere s);

int shadow(vec v, light l, sphere_list *ss);

/* as a warm-up, implement specular reflection in lighting */
rgb lighting(scene s, ray r, hit_test h);

vec logical_loc(camera c, vec pos);

rgb trace_ray(scene s, ray r);

void render(stage g);

/* === project 2 operations === */

/* fintersect is like intersect, but uses the timestamp */
/* to compute the surface color and specular reflection */
/* color of the intersecting fsphere (if there is one)  */
hit_test fintersect(ray r, fsphere s, timestamp ts);

/* fshadow is like shadow, but needs to pass a timestamp */
/* to fintersect                                         */
int fshadow(vec p, light l, fsphere_list *ss, timestamp ts);

/* flighting is like lighting, but consumes a frame */
/* rather than a scene                              */
rgb flighting(frame f, ray r, hit_test h);

/* ftrace is like trace, but for frames rather than scenes */
rgb ftrace(ray r, frame f);

/* frender consumes a camera and a frame and renders the frame, */
/* writing a ppm file to the given filename                     */
/* - use fopen to open the file, and fclose when you're done    */
void frender(camera c, frame f, char *filename);

/* frender_frames renders all frames given an fstage and writes them to files */
/* note: the string name_stem determines the file names */
/* ex: if name_stem is "myframe" and there are 100 frames to render, the filenames */
/*     should be "myframe00.ppm", "myframe01.ppm", ... "myframe99.ppm" */
/* ex: if name_stem is "zz" and there are 100 frames to render, the filenames */
/*     should be "zz00.ppm", "zz01.ppm", ... "zz99.ppm" */
/* note: you *may* include additional leading zeroes if you prefer, just make */
/*       sure the number of digits in each filename is constant, and that     */
/*       files are numbered consecutively */
/*       - this means the following are both OK: */
/*         "zz00.ppm", "zz01.ppm", ... "zz99.ppm" */
/*         "zz0000.ppm", "zz0001.ppm", ... "zz0099.ppm" */
void frender_frames(fstage g, char *name_stem);

#endif /* __RAYTRACER_H__ */
