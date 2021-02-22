#include "game_object.h"

int default_is_colliding (game_object* this, game_object* other) {
	if (rectangle_overlaps (&(this->hitbox), &(other->hitbox))) {
		return 1;
	}
	return 0;
}

void generate_hitbox (game_object* obj) {
	make_rectangle (&(obj->hitbox), obj->x, obj->y, obj->width, obj->height);
}

game_object* make_game_object (void* ptr, char* obj_type) {
	game_object* obj = (game_object*)ptr;
	obj->object_type = obj_type;
	obj->vertex_data = alloc_render_object ();
	obj->init_call = default_init;
	obj->draw_call = default_draw;
	obj->game_logic_call = default_game_logic;
	obj->is_colliding = default_is_colliding;
	return obj;
}

void default_init (game_object* obj) {
	//THIS IS FOR TESTING
	obj->sprite = make_sprite ("testure.png");
	obj->x = 0;
	obj->y = 0;
	obj->width = .5;
	obj->height = .5;
}

void default_game_logic (game_object* obj) {
	//Intentionally left empty
}

void default_draw (game_object* obj) {
	if (obj->sprite) {
		
		//Get the object's position and sprite data
		rectangle* r = &(obj->sprite->mapping->bounds);
		unsigned int tex_id = obj->sprite->mapping->tex_id;
		
		//Update the object's vertex data
		//Top-left
		obj->vertex_data[0] = obj->x;
		obj->vertex_data[1] = obj->y;
		obj->vertex_data[3] = r->x;
		obj->vertex_data[4] = r->y;
		obj->vertex_data[5] = tex_id;
		//Bottom-left
		obj->vertex_data[6] = obj->x;
		obj->vertex_data[7] = obj->y + obj->height;
		obj->vertex_data[9] = r->x;
		obj->vertex_data[10] = r->y + r->height;;
		obj->vertex_data[11] = tex_id;
		//Bottom-right
		obj->vertex_data[12] = obj->x + obj->width;
		obj->vertex_data[13] = obj->y + obj->height;
		obj->vertex_data[15] = r->x + r->width;
		obj->vertex_data[16] = r->y + r->height;
		obj->vertex_data[17] = tex_id;
		//Top-right
		obj->vertex_data[18] = obj->x + obj->width;
		obj->vertex_data[19] = obj->y;
		obj->vertex_data[21] = r->x + r->width;
		obj->vertex_data[22] = r->y;
		obj->vertex_data[23] = tex_id;
	}
}