#include "raytracer.h"

char *hit_test_tos(hit_test t)
{
  char buf[1024] = {0};
  char *sc, *sh, *sn;
  if (t.miss) {
    return strdup("hit_test(MISS)");
  }
  sc = rgb_tos(t.surf);
  sh = rgb_tos(t.shine);
  sn = vec_tos(t.surf_norm);
  sprintf(buf,"hit_test(%lf,%s,%s,%s)",t.dist,sc,sh,sn);
  free(sc);
  free(sh);
  free(sn);
  return strdup(buf);
}

void hit_test_print(hit_test t)
{
  char *s = hit_test_tos(t);
  printf("%s\n",s);
  free(s);
}
