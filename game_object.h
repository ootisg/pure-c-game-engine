#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <stdlib.h>

#include <GL/glew.h>
#include <GL/glut.h>

#include "main.h"
#include "geometry.h"

struct animation_handler {
	int frame;
	float animation_speed;
};

/// A struct representing a game object (also woefully underdocumented)
struct game_object {
	char* object_type;
	void* object_data;
	GLfloat* vertex_data;
	void (*init_call)(struct game_object*);
	void (*draw_call)(struct game_object*);
	void (*game_logic_call)(struct game_object*);
	int (*is_colliding)(struct game_object*, struct game_object*);
	sprite* sprite;
	struct animation_handler animator;
	double x;
	double y;
	double width;
	double height;
	rectangle hitbox;
};

typedef struct game_object game_object;
typedef struct animation_handler animation_handler;

/// Makes a new game_object at the given location
/// @param ptr the location to make the game_object at
/// @param type the type name of the game_object
/// @return ptr cast to a game_object*
game_object* make_game_object (void* ptr, char* type);

/// Initializes a new animation_handler at the location ptr
/// @return ptr cast to animation_handler*
animation_handler* make_animation_handler (void* ptr);

/// The default init function for a game_object
/// @param obj the game_object running this function
void default_init (game_object* obj);

/// The default game logic function for a game_object
/// @param obj the game_object running this function
void default_game_logic (game_object* obj);

/// The default draw function for a game_object
/// @param obj the game_object running this function
void default_draw (game_object* obj);

/// The default hitbox generation function for game_object, which uses its x, y, width, and height
/// @param obj the game_object whose hitbox will be initialized
void generate_hitbox (game_object* obj);

#endif