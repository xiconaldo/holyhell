#version 430 core

in vec3 norm_c;
out vec4 color;
layout (location = 3) uniform vec3 light = vec3(0.0f, 0.0f, -1.0f);
vec3 local_light;
float intensity;

void main(){
	local_light = normalize(light);
	intensity = -dot(norm_c, local_light);
	if(intensity < 0.2) intensity = 0.2;
	color = vec4(0.0f, 0.6f, 0.2f, 1.0f) * pow(intensity, 2);
}