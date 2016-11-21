#version 430 core

layout (binding  = 0) uniform sampler2D base_texture;

in  vec2 text_coord;
out vec4 color;

void main(){
	color = texture(base_texture, text_coord);
}