#ifndef GEOMETRY_H
#define GEOMETRY_H

struct rectangle {
	float x;
	float y;
	float width;
	float height;
};

typedef struct rectangle rectangle;

void print_rectangle (rectangle* r);

#endif