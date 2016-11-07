#version 430 core

layout (binding  = 0) uniform sampler2D base_texture;

in vec3 norm_coord;
in vec2 text_coord;
out vec4 color;
vec4 algo;

vec3 light = vec3(-1.0f, -1.0f, -1.0f);
//vec3 light = vec3(0.0f, -1.0f, 0.0f);

float intensity;

void main(){
	light = normalize(light);
	intensity = -dot(norm_coord, light);
	if(intensity < 0.4) intensity = 0.4;

	vec4 text_color = texture(base_texture, text_coord);
	color = vec4(text_color.xyz * pow(intensity, 2), text_color.a);
}