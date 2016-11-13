#version 430 core

layout (location = 3) uniform vec3 light = vec3(-1.0f, -1.0f, -1.0f);
layout (binding  = 0) uniform sampler2D base_texture;

in  vec3 norm_coord;
in  vec2 text_coord;
out vec4 color;

vec3 local_light;
float intensity;

void main(){
	local_light = normalize(light);
	intensity = -dot(norm_coord, local_light);
	if(intensity < 0.2) intensity = 0.2;

	vec4 text_color = texture(base_texture, text_coord);
	color = vec4(text_color.xyz * pow(intensity, 2), text_color.a);
}