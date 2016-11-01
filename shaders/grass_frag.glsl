#version 430 core

layout (location = 3) uniform vec3 light = vec3(0.0f, 0.0f, -1.0f);
layout (binding = 0)  uniform sampler2D object_texture;

in 	vec3 norm_coord;
in 	vec2 text_coord;
out vec4 color;

vec3 local_light;
float intensity;

void main(){
	local_light = normalize(light);
	intensity = -dot(norm_coord, local_light);
	if(intensity < 0.2) intensity = 0.2;
	//color = vec4(0.0f, 0.6f, 0.2f, 1.0f) * pow(intensity, 2);
	color = texture(object_texture, text_coord) * pow(intensity, 2);
}