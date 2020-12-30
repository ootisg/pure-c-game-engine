#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;
layout (location = 2) in uint texid;

out vec2 texcoord_final;
out uint texid_final;

void main()
{
    gl_Position = vec4(position, 1.0);
	texcoord_final = texcoord;
	texid_final = texid;
}