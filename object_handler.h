#ifndef OBJECT_HANDLER_H
#define OBJECT_HANDLER_H

#include <stdlib.h>

#include "linked_list.h"
#include "hash_table.h"
#include "game_object.h"

///Documentation coming soon to a header file near you

struct object_handler {
	linked_list* objects;
	hash_table* objects_map;
};

typedef struct object_handler object_handler;

object_handler* get_global_object_handler ();
object_handler* make_object_handler (void* ptr);
int declare_game_object (object_handler* obj_handler, game_object* obj);
void run_objs_game_logic (object_handler* obj_handler);
void run_objs_draw (object_handler* obj_handler);

#endif