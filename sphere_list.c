#include "raytracer.h"

sphere_list *sl_cons(sphere s, sphere_list *ss)
{
  sphere_list *new = malloc(sizeof(sphere_list));
  if (new==NULL) {
    fprintf(stderr,"sl_cons: malloc failed\n");
    exit(1);
  }
  new->s = s;
  new->next = ss; /* note: shallow copy */
  return new;
}

fsphere_list *fsl_cons(fsphere s, fsphere_list *ss)
{
  fsphere_list *new = malloc(sizeof(fsphere_list));
  if (new==NULL) {
    fprintf(stderr,"fsl_cons: malloc failed\n");
    exit(1);
  }
  new->s = s;
  new->next = ss; /* note: shallow copy */
  return new;
}

sphere_list *sl_singleton(sphere s)
{
  return sl_cons(s,NULL);
}

fsphere_list *fsl_singleton(fsphere s)
{
  return fsl_cons(s,NULL);
}

void sl_print(sphere_list *ss)
{
  printf("[BEGIN sphere list]\n");
  while (ss) {
    printf("+ ");
    sphere_print(ss->s);
    ss = ss->next;
  }
  printf("[END sphere list:\n");
}

void sl_free(sphere_list *ss)
{
  if (ss!=NULL) {
    sl_free(ss->next);
    free(ss);
  }
}
