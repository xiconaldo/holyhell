#version 430 core

layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 text_coord;

out vec2 coord;

void main(){
	vec4 pos = vec4(vertex, 0.0f, 1.0f);

	gl_Position = pos;

	coord = text_coord;
}