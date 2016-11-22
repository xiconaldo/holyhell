#version 430 core

layout (location = 7) uniform float desat = 0.0f;

layout (binding  = 0) uniform sampler2D base_texture;

in  vec2 text_coord;
in float I_factor;
out vec4 color;

vec3 local_light_dir;
float intensity;

void main(){

	vec4 text_color = texture(base_texture, text_coord);
	color = vec4(text_color.xyz * I_factor, text_color.a);

	float bw = (color.r + color.g + color.b)*0.333f;
	vec3 bw_color = vec3(bw);

	color.rgb = (bw_color - color.rgb)*desat + color.rgb;

}