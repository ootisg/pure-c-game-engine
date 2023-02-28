This is a simple 2D game engine I'm making for fun. If you want to use it, go ahead (but do so at your own risk as it's unfinished, and probably won't be seeing any updates for a little while).

Building:  
  
This library has the following dependancies:  
	- Must be compiled with MinGW
	- Win32 (this is a WINDOWS ONLY library)  
	- OpenGL version 3.0 or higher  
	- GLU  
	- freeglut  
	- GLEW (put the source glew.c file in the project directory, header can go wherever as long as it's accessable)  
	- lodepng (put the .c and .h files in the project directory)  
  
Steps to build:  
	Note: This project includes a lib and an include directory in the source folder. Libraries/headers can either be placed here, or in your system's lib/include folders. It doesn't particularly matter which.
	1) Download and install OpenGL if you haven't already  
	2) Make sure you have MinGW installed along with GNUMake.  
	3) Download and install GLU and freeglut (these libraries should be in your lib directory)  
	4) Download glew.c and glew.h, and put glew.c in the project's source directory. glew.h can go in your include directory.  
	5) Download lodepng.c and lodepng.h, and put them both in the project's source directory.  
	6) Build with gnu make using the provided Makefile  
	  
How to use:  

The entry point is ```game_init()``` in ```main.c```. Defer initialization to your own source file, or just write all of your code inside the function. Either will work.

This engine is based on an event/callback model. You create ```game_object``` structs, populate them, then register them with an ```object_handler``` using ```declare_game_object()```.  
The ```game_object``` struct has three callback functions:  
	- ```init_call()``` is called by ```declare_game_object()``` as the object's initialization step.  
	- ```draw_call()``` is called once per frame (after ```game_logic_call()``` is called for all ```game_object``` structs), and is responsible for modifying data used for rendering the object.  
	- ```game_logic_call()``` is called once per frame, and is responsible for the object's behavior and logic.  
All of these callbacks take one argument of type ```game_object*```. When a ```game_object``` struct's callbacks are called, the struct is passed as the argument.  
  
The ```x```, ```y```, ```width```, ```height```, ```sprite```, and ```animation_handler``` are the only fields of a ```game_object``` struct that affect how it is rendered. These allow for basic functionality:  
	- Objects can be translated and scaled by modifying the ```x```, ```y```, ```width```, and ```height``` attributes.  
	- An object's texture can be changed by modifying the ```sprite``` field.  
	- Sprites can have multiple frames. Which frame is rendered is handled by the ```animation_handler``` field.  
For a game_object to be rendered, it must be registered with the global object handler. This can be done like so: ```declare_game_object (get_global_object_handler(), <obj>);```  
  
Sprites can be created using either ```make_sprite()``` or ```make_sprite_from_json()```. The former call makes a one-frame sprite using an image filepath; the latter uses an image and a JSON config file to parse a sprite into animation frames.  
The JSON formatting is as follows:  
	The file has one optional field and one required field.  
	```"sprite_path"``` is an optional field that contains the image filepath for the sprite.  
	```"layout"``` is a required array field that contains the frame layout for the sprite. It contatins one or more JSON objects of the following format:  
	```{  
		"type": - can be either "rect" or "grid". Elements of type "grid" specify a grid to fill the area, with cells of size "width" and "height". "rect" specifies one single rectangle.  
		"width": - the width of the frame(s)  
		"height": - the height of the frame(s)  
		"x": - the x-position of the frame source (only used for "rect" elements)  
		"y": - the y-position of the frame source (only used for "rect" elements)  
		"layout" - a layout to apply within the frame(s) to parse subframes. (Note: this field makes the format entirely recursive.)  
	}```  
	Note that the dimensions used for these fields are relative to the size of their container. This starts at the top level as the top-left corner of the sprite being (0,0) and the bottom-right being (1,1). This means that for images or recursive bounds that don't have an equal width and height, x and y are on different scales and a frame with width=height might not be square.
	Some example files are included below:  
	  
	{
	"layout":[
		{
			"type":"grid",
			"width":0.25,
			"height":1.0
		}
	]
	}
	This file creates 4 frames arranged sequentially, each one being 1/4 the width of the image file and taking the entire height of the image. For example, using this on a 128x32px spritesheet would yield 4 32x32 frames.  
	  
	{
	"layout":[
		{
			"type":"rect",
			"x":0.0,
			"y":0.0,
			"width":0.25,
			"height":1.0
		},
		{
			"type":"rect",
			"x":0.25,
			"y":0.0,
			"width":0.25,
			"height":1.0
		},
		{
			"type":"rect",
			"x":0.0,
			"y":0.0,
			"width":0.25,
			"height":1.0
		},
		{
			"type":"rect",
			"x":0.75,
			"y":0.0,
			"width":0.25,
			"height":1.0
		}
	]
	}
	Behaves identically to the previous example.
	  
	{
	"layout":[
		{
			"type":"rect",
			"x":0.25,
			"y":0.25,
			"width":0.5,
			"height":0.5
		}
	]
	}
	This file creates 1 frame from the middle of the image, with a border of 1/4 the image width and height cropped out. Using this on a 64x64 image would result in a 32x32 frame taken from the center of the image.  
	  
	{
	"layout":[
		{
			"type":"rect",
			"x":0.0,
			"y":0.0,
			"width":1.0,
			"height":0.25,
			"layout":[
				{
					"type":"grid",
					"width":0.25,
					"height":1.0
				}
			]
		}
	]
	}
	This file takes the top 1/4 of the image as a horizontal slice, then splits it into 4 sequential frames. Using this on a 128x128 image would result in four 32x32 frames lined up at the top edge of the image.  
	  
	{
	"layout":[
		{
			"type":"grid",
			"width":0.125,
			"height":0.125,
			"layout":[
				{
					"type":"rect",
					"x":0.25,
					"y":0.25,
					"width":0.5,
					"height":0.5
				}
			]
		}
	]
	}
	This file splits the image into an 8x8 grid, then takes the center of each cell and cuts off a 25% wide border. Using this on a 256x256 image would yield 64 16x16 frames. This type of layout is particularly useful for spritesheets with borders or padding between the sprites.  
	  
	{
	"source_path":"resources/sprites/slime_idle.png",
	"layout":[
		"type":"grid",
		"width":0.125,
		"height":1.0
	]
	}
	This file uses the optional source_path parameter to specify an image path. The idle animation spritesheet is parsed into 8 frames lined up horizontally.  
  
To get player inputs, use the ```get_inputs()``` function. This returns a struct of the type ```input_state```, which contains info about which keys are held down, which were just pressed or released, and some state information about mouse inputs.  
  
This game engine also comes with implementations of simple data structures in ```linked_list.c```, ```char_buffer.c```, ```hash_map.c```, and ```stack.c```. Refer to the corresponding header files for documentation.  
  
Additional notes:  
	- Audio playback is not currently supported. It will likely be added eventually.  
	- Sprites are stitched onto a 1024x1024 texture. Currently, only one texture is used; attempting to allocate additional sprites will fail once it is full. This will be addressed in the future.  
	- There's a strange bug that causes some sprites to be slightly misaligned on certain hardware. These show up as seams and are not very pretty. I've tested this a fair bit and it doesn't seem to be a floating point precision error. If anyone knows how to fix this, please let me know.  