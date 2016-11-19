#version 430 core

layout (binding  = 0) uniform sampler2D base_texture;
layout (binding  = 5) uniform sampler2D height_map;

in 	vec2 text_coord;
out vec4 color;
in  float intensity;

void main(){

	vec4 text_color = texture(base_texture, text_coord*50.0f);
	color = vec4(text_color.xyz * pow(intensity, 2), text_color.a);
}