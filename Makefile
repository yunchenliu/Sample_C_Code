# Makefile (starter)

# CS152 Winter 2014
# Project 2
# Feb 28 2014

.PHONY: clean

SRC = vec.c rgb.c ray.c sphere.c sphere_list.c light.c \
      scene.c camera.c hit_test.c stage.c raytracer.c

trace: raytracer.h $(SRC)
	gcc -Wall -o trace $(SRC) -lm

clean:
	rm -rf trace

