#include <stdlib.h>

#include "sprite.h"

sprite* make_sprite (char* filepath) {
	
	//Read from the input file
	sprite* out_sprite = malloc (sizeof (sprite));
	int width, height;
	unsigned char* img;
	lodepng_decode32_file (&img, &width, &height, filepath);
	
	//Transfer the sprite data to a sprite texture
	texture_mapping* map = get_texture_mapping (width, height);
	out_sprite->mapping = map;
	transfer_to_texture (map, img, width, height);
	
	//Set the sprite properties
	out_sprite->image_data = img;
	out_sprite->texture_id = map->tex_id;
	out_sprite->frame_count = 0; //TODO
	out_sprite->width_px = width;
	out_sprite->height_px = height;
	out_sprite->width = map->bounds.width;
	out_sprite->height = map->bounds.height;
	out_sprite->frames = (rectangle*)0;
	return out_sprite;
}

void transfer_to_texture (texture_mapping* map, unsigned char* source, int source_width, int source_height) {

	//Setup the important data
	rectangle* bounds = &(map->bounds);
	unsigned int tex_id = map->tex_id;
	char* dest = get_sprite_texture (tex_id);
	int* source_int = (int*)source;
	int* dest_int = (int*)dest;
	
	//Calculate the x bounds
	int x1 = (int)(bounds->x * TEXTURE_SIZE);
	int y1 = (int)(bounds->y * TEXTURE_SIZE); //Flip the y position to accomodate texture stuff
	int x2 = x1 + source_width; //TODO unused
	int y2 = y2 - source_height;
	
	//Copy over and flip the output image vertically
	int wx, wy;
	for (wy = 0; wy < source_height; wy++) {
		for (wx = 0; wx < source_height; wx++) {
			dest_int[(wy + y1) * TEXTURE_SIZE + wx + x1] = source_int [(source_height - wy - 1) * source_width + wx];
		}
	}
}