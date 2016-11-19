#version 430 core

layout (binding  = 0) uniform sampler2D base_texture;

in  vec2 text_coord;
out vec4 color;
in float intensity;

void main(){

	vec4 text_color = texture(base_texture, text_coord);
	color = vec4(text_color.xyz * pow(intensity, 2), text_color.a);
}