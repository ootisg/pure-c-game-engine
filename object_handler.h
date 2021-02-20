#ifndef OBJECT_HANDLER_H
#define OBJECT_HANDLER_H

#include <stdlib.h>

#include "linked_list.h"
#include "hash_table.h"
#include "game_object.h"

///Struct for handling/tracking game_objects
struct object_handler {
	linked_list* objects;
	hash_table* objects_map;
};

///nice
typedef struct object_handler object_handler;

///Gets the global object handler, because putting this in main.h creates a circular dependancy
object_handler* get_global_object_handler ();

///Initializes an object_handler struct at the given pointer and returns it.
///@param ptr the location to initialize the object_handler
///@return ptr cast to object_handler*
object_handler* make_object_handler (void* ptr);

///Declares the game_obj obj to the given obj_handler. This allows obj_handler to track obj.
///@param obj_handler the object_handler to declare to
///@param obj the object to declare
///@return always returns 0
int declare_game_object (object_handler* obj_handler, game_object* obj);

///Runs game_logic_call for every game_object tracked by obj_handler.
///@param obj_handler the object_handler to use
void run_objs_game_logic (object_handler* obj_handler);

///Runs draw_call for every game_object tracked by obj_handler.
///@param obj_handler the object_handler to use
void run_objs_draw (object_handler* obj_handler);

#endif