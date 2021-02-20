#include <stdio.h>

#include "geometry.h"

rectangle* make_rectangle (void* ptr, float x, float y, float width, float height) {
	rectangle* r_ptr = (rectangle*)ptr;
	r_ptr->x = x;
	r_ptr->y = y;
	r_ptr->width = width;
	r_ptr->height = height;
	return r_ptr;
}

void print_rectangle (rectangle* r) {
	printf ("RECTANGLE: [x: %f, y: %f, width: %f, height: %f]\n", r->x, r->y, r->width, r->height);
}

int rectangle_contains (rectangle* rect, float x, float y) {
	if (x >= rect->x && x < (rect->x + rect->width) && y >= rect->y && y < (rect->y + rect->height)) {
		return 1; //True
	}
	return 0; //False
}

int rectangle_overlaps (rectangle* r1, rectangle* r2) {
	printf ("%f,%f\n", r1->y, r2->y - r1->height);
	if (r1->x > r2->x - r1->width && r1->x < r2->x + r2->width && r1->y > r2->y - r1->height && r1->y < r2->y + r2->height) {
		return 1;
	}
	return 0;
}