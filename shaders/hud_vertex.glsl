#version 430 core

// layout (location = 0) in vec2 vertex;
// layout (location = 2) in vec2 uv;

vec2 vertex[] = {vec2(-1.0f, 1.0f), vec2(-1.0f, -1.0f), vec2(1.0f, -1.0f),
				 vec2( 1.0f, 1.0f), vec2(-1.0f, -1.0f), vec2(1.0f, -1.0f)};

vec2 uv[] 	  = {vec2(0.0f, 1.0f), vec2(0.0f, 0.0f), vec2(1.0f, 0.0f),
				 vec2(1.0f, 1.0f), vec2(0.0f, 0.0f), vec2(1.0f, 0.0f)};

out vec2 text_coord;

void main(){
	vec4 pos = vec4(vertex[gl_VertexID], 0.0f, 1.0f);

	gl_Position = pos;
	
	text_coord = uv[gl_VertexID];
}