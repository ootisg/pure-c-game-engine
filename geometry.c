#include <stdio.h>

#include "geometry.h"

void print_rectangle (rectangle* r) {
	printf ("RECTANGLE: [x: %f, y: %f, width: %f, height: %f]\n", r->x, r->y, r->width, r->height);
}