#include "main.h"

#include "game_object.h"
#include "linked_list.h"
#include "hash_table.h"

//This file is a WIP

GLfloat g_ex_vertex_buffer_data[] = { 
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0i,
     1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0i,
    -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0i,
     1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 0i
};
GLuint g_ex_element_buffer_data[] = { 0, 1, 2, 3 };

GLfloat* g_vertex_buffer_data = g_ex_vertex_buffer_data;
GLuint* g_element_buffer_data = g_ex_element_buffer_data;

int g_vertex_buffer_fill;

stack* g_free_objects;

game_object* test_obj;

struct {
	GLuint vertex_buffer;
	GLuint element_buffer;
	GLuint vertex_shader;
	GLuint frag_shader;
	GLuint program;
	GLuint texture;
	struct {
		GLuint position;
		GLuint tex;
		GLuint texid;
	} attributes;
	struct {
		GLuint texture;
	} uniforms;
} g_resources;

void show_info_log(
    GLuint object,
    PFNGLGETSHADERIVPROC glGet__iv,
    PFNGLGETSHADERINFOLOGPROC glGet__InfoLog
)
{
	//COPY PASTED
    GLint log_length;
    char *log;

    glGet__iv(object, GL_INFO_LOG_LENGTH, &log_length);
    log = malloc(log_length);
    glGet__InfoLog(object, log_length, NULL, log);
    fprintf(stderr, "%s", log);
    free(log);
}

void init_vertex_buffers () {
	g_vertex_buffer_data = malloc (sizeof (GLfloat) * 24 * MAX_OBJECTS);
	g_element_buffer_data = malloc (sizeof (GLfloat) * MAX_OBJECTS);
}

GLfloat* alloc_render_object () {
	if (stack_size (g_free_objects)) {
		return *((GLfloat**)pop (g_free_objects));
	} else {
		int i;
		for (i = 0; i < 4; i++) {
			g_element_buffer_data[g_vertex_buffer_fill * 4 + i] = g_vertex_buffer_fill * 4 + i;
		}
		return g_vertex_buffer_data + RENDER_OBJECT_SIZE * g_vertex_buffer_fill++;
	}
}

GLchar* file_contents (const char* filename, int* length) {
	//Open file
	FILE* f = fopen (filename, "r");
	
	//Get the file length
	fseek (f, 0, SEEK_END);
	*length = ftell (f);
	fseek (f, 0, SEEK_SET);
	
	GLchar* fcontents = malloc ((*length + 1) * sizeof(GLchar));
	//Make a buffer and read the file to it
	int i;
	for (i = 0; i < *length; i++) {
		int read_char = fgetc (f);
		if (read_char == EOF) {
			*length = i;
			fcontents[i] = 0; //Add a null terminator
			break;
		}
		fcontents[i] = (GLchar)read_char;
	}
	printf ("%d\n%s\n", *length, fcontents);
	
	//Return the buffer ptr
	return fcontents;
}

GLuint make_buffer (GLenum target, const void* buffer_data, GLsizei buffer_size) {
	GLuint buffer;
	glGenBuffers (1, &buffer);
	glBindBuffer (target, buffer);
	glBufferData (target, buffer_size, buffer_data, GL_STATIC_DRAW);
	return buffer;
}

GLuint make_texture_from_buffer (char* buffer, int width, int height) {
	GLuint texture;
	glGenTextures (1, &texture);
	glBindTexture (GL_TEXTURE_2D, texture);
	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glTexImage2D (GL_TEXTURE_2D, 	//Target
				  0,				//Mipmap Level
				  GL_RGBA8,			//Format
				  width, height,	//Width and Height
				  0,				//Well okay then
				  GL_RGBA, 			//Ordering(?)
				  GL_UNSIGNED_BYTE,	//Type of the pixel data
				  buffer			//The image data
				  );
				  
	return texture;
}

GLuint make_texture (char* filename) {
	//Read from file then make the texture
	int width, height;
	unsigned char* img;
	lodepng_decode32_file (&img, &width, &height, filename);
	GLuint tex = make_texture_from_buffer (img, width, height);
	free (img);
	return tex;
}

GLuint make_shader (GLenum type, const char* filename) {
	
	//Read in the shader
	GLint length;
	GLchar* source = file_contents (filename, &length);
	GLuint shader;
	GLuint shader_ok;
	
	//Terminate if file reading failed
	if (!source) {
		return 0;
	}
	
	//Make the shader
	shader = glCreateShader (type);
	glShaderSource (shader, 1, (const GLchar**)&source, &length);
	free (source);
	glCompileShader (shader);
	
	//Handle any shader related mishaps
	glGetShaderiv (shader, GL_COMPILE_STATUS, &shader_ok);
	if (!shader_ok) {
		//Copy-pasted, don't judge
		fprintf(stderr, "Failed to compile %s:\n", filename);
		show_info_log(shader, glGetShaderiv, glGetShaderInfoLog);
        glDeleteShader(shader);
        return 0;
	}
	
	//RETURN THE SHADER (IMPORTANT!)
	return shader;
}

GLuint make_program (GLuint vertex_shader, GLuint frag_shader) {
	GLint program_ok;
	GLuint program = glCreateProgram ();
	glAttachShader (program, vertex_shader);
	glAttachShader (program, frag_shader);
	glLinkProgram (program);
	//Handle any program related mishaps (copy-pasted, oof)
	glGetProgramiv(program, GL_LINK_STATUS, &program_ok);
    if (!program_ok) {
        fprintf(stderr, "Failed to link shader program:\n");
        show_info_log(program, glGetProgramiv, glGetProgramInfoLog);
        glDeleteProgram(program);
        return 0;
    }
    return program;
}

void refresh_buffers () {
	g_resources.vertex_buffer = make_buffer (GL_ARRAY_BUFFER, g_vertex_buffer_data, g_vertex_buffer_fill * 24 * sizeof (GLfloat));
	g_resources.element_buffer = make_buffer (GL_ARRAY_BUFFER, g_element_buffer_data, g_vertex_buffer_fill * 4 * sizeof (GLfloat));
}

void refresh_textures () {
	g_resources.texture = make_texture_from_buffer (get_sprite_texture (0), TEXTURE_SIZE, TEXTURE_SIZE);
}

int make_resources () {
	init_vertex_buffers ();
	refresh_buffers ();
	//memcpy (g_vertex_buffer_data, g_ex_vertex_buffer_data, 96);
	//memcpy (g_element_buffer_data, g_ex_element_buffer_data, 16);
	//g_vertex_buffer_fill = 4;
	//g_resources.texture = make_texture ("testure.png");
	
	//Make the texture
	refresh_textures ();
	
	//Make and bind the shaders
	g_resources.vertex_shader = make_shader (GL_VERTEX_SHADER, "vertex_shader.glsl");
	g_resources.frag_shader = make_shader (GL_FRAGMENT_SHADER, "fragment_shader.glsl");
	g_resources.program = make_program (g_resources.vertex_shader, g_resources.frag_shader);
	
	//Find the uniforms
	g_resources.uniforms.texture = glGetUniformLocation (g_resources.program, "texture");
	
	//Find the attributes
	g_resources.attributes.position = glGetAttribLocation (g_resources.program, "position");
	g_resources.attributes.tex = glGetAttribLocation (g_resources.program, "texcoord");
	g_resources.attributes.texid = glGetAttribLocation (g_resources.program, "texid");
	printf ("Attributes handle: %d\n", g_resources.attributes.position);
}

void init () {
	make_resources ();
}

void render () {
	default_draw (test_obj);
	refresh_buffers ();
	refresh_textures ();
	glUseProgram (g_resources.program);
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_resources.texture);
    glUniform1i(g_resources.uniforms.texture, 0);
	glBindBuffer(GL_ARRAY_BUFFER, g_resources.vertex_buffer);
	GLuint stride_len = sizeof(GLfloat) * 6;
	GLuint offset = 0;
	//Map the position
    glVertexAttribPointer(
        g_resources.attributes.position,  // attribute
        3,                                // size 
        GL_FLOAT,                         // type 
        GL_FALSE,                         // normalized? 
        stride_len,                       // stride 
        (void*)offset                     // array buffer offset 
    );
	glEnableVertexAttribArray(g_resources.attributes.position);
	//Map the texture coordinates
	glVertexAttribPointer (g_resources.attributes.tex, 2, GL_FLOAT, GL_FALSE, stride_len, (void*)(offset += sizeof (GLfloat) * 3));
	glEnableVertexAttribArray (g_resources.attributes.tex);
	//Map the texture ID
	glVertexAttribPointer(g_resources.attributes.texid, 1, GL_UNSIGNED_INT, GL_FALSE, stride_len, (void*)(offset += sizeof (GLfloat) * 2));
	glEnableVertexAttribArray (g_resources.attributes.texid);
	//Bind the vertex buffer and render
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_resources.element_buffer);
    glDrawElements(
        GL_QUADS,			      	// mode
        g_vertex_buffer_fill * 4,	// count
        GL_UNSIGNED_INT,        	// type
        (void*)0            		// element array buffer offset
    );
	glDisableVertexAttribArray(g_resources.attributes.position);
	glutSwapBuffers ();
}

void display () {
	//Do nothing, this is gay
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	render ();
	glutSwapBuffers ();
	int i = 0;
	while (i < 20000000) {
		i++;
	}
	swap_input_buffers ();
	//printf ("frame");
	//glutPostRedisplay ();
}

void test () {
	g_free_objects = init_stack (sizeof (GLfloat*));
	void* obj_ptr = malloc (sizeof (game_object));
	test_obj = make_game_object (obj_ptr, "TEST");
	default_init (test_obj);
	
	//Test the hash table
	int data[] = {2, 5, 6, 9, 15, 3, 20};
	int data2 = 5;
	hash_table* table = make_hash_table (malloc (sizeof (hash_table)));
	hash_table_put (table, "NUM_TWO", 8, &data);
	hash_table_put (table, "NUM_FIVE", 9, &data[1]);
	hash_table_put (table, "NUM_SIX", 8, &data[2]);
	hash_table_put (table, "NUM_NINE", 9, &data[3]);
	hash_table_put (table, "NUM_FIFTEEN", 12, &data[4]);
	hash_table_put (table, "NUM_THREE", 10, &data[5]);
	hash_table_put (table, "NUM_TWENTY", 11, &data[6]);
	printf ("Val: %d\n", (int)hash_table_get (table, "NUM_TWO", 8));
	printf ("Val: %d\n", (int)hash_table_get (table, "NUM_FIVE", 9));
	printf ("Val: %d\n", (int)hash_table_get (table, "NUM_SIX", 8));
	printf ("Val: %d\n", (int)hash_table_get (table, "NUM_NINE", 9));
	printf ("Val: %d\n", (int)hash_table_get (table, "NUM_FIFTEEN", 12));
	printf ("Val: %d\n", (int)hash_table_get (table, "NUM_THREE", 10));
	printf ("Val: %d\n", (int)hash_table_get (table, "NUM_TWENTY", 11));
}
  
int main (int argc, char** argv) {
	printf ("HELLO WORLD");
	//TEST LODEPNG
	unsigned char* img;
	int width;
	int height;
	lodepng_decode32_file (&img, &width, &height, "testure.png");
	//TEST END
	printf ("glchar %d\n", sizeof(char));
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow ("Hello World");
	glewInit ();
	printf ("OpenGL %s\n", glGetString (GL_VERSION));
	init_inputs ();
	glutDisplayFunc (render);
	glutKeyboardFunc (key_down_handler);
	glutKeyboardUpFunc (key_up_handler);
	glutMouseFunc (mouse_handler);
	glutMotionFunc (mouse_motion_handler);
	glutPassiveMotionFunc (passive_mouse_motion_handler);
	init ();
	test ();
	glutMainLoop ();
	return 0;
}