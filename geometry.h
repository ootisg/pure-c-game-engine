#ifndef GEOMETRY_H
#define GEOMETRY_H

///About what you'd expect
struct rectangle {
	float x;
	float y;
	float width;
	float height;
};

typedef struct rectangle rectangle;

///Initializes a rectangle struct at the given pointer with the given attributes
///@param ptr the location to initialize the rectangle
///@param x the x-coordinate of the rectangle
///@param y the y-coordinate of the rectangle
///@param width the width of the rectangle
///@param height the height of the rectangle
///@return ptr cast to rectangle*
rectangle* make_rectangle (void* ptr, float x, float y, float width, float height);

///Checks if rect contains the point (x, y)
///@param rect the rectangle to check
///@param x the x-coordinate of the point
///@param y the y-coordinate of the point
///@return 1 if rect contains (x, y); 0 otherwise
int rectangle_contains (rectangle* rect, float x, float y);

///Checks if the 2 given rectangles overlap, order doesn't matter
///@param r1 a rectangle to check
///@param r2 a rectangle to check
///@return 1 if the rectangles overlap; 0 otherwise
int rectangle_overlaps (rectangle* r1, rectangle* r2);

///Prints the given rectangle to the console
void print_rectangle (rectangle* r);

#endif