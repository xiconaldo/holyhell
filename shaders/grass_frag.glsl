#version 430 core

in vec3 norm_c;
out vec4 color;
vec3 light = vec3(-1.0f, -1.0f, -1.0f);
float intensity;

void main(){
	light = normalize(light);
	intensity = -dot(norm_c, light);
	if(intensity < 0.1) intensity = 0.1;
	color = vec4(0.0f, 1.0f, 0.4f, 1.0f) * intensity;
}