#version 430 core

layout (location = 7) uniform float desat = 0.0f;

layout (binding  = 0) uniform sampler2D base_texture;
layout (binding  = 5) uniform sampler2D height_map;

in 	vec2 text_coord;
out vec4 color;
in float I_factor;

void main(){
	

	vec4 text_color = texture(base_texture, text_coord*50.0f);
	color = vec4(text_color.xyz * I_factor * I_factor, text_color.a);

	float bw = (color.r + color.g + color.b)*0.333f;
	vec3 bw_color = vec3(bw);

	color.rgb = (bw_color - color.rgb)*desat + color.rgb;

	// if(I_factor < 0.1f) color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	// else if(I_factor > 0.5f) color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}