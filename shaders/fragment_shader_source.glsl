#version 430 core

out vec4 color;
in vec4 n_out;
vec4 light;
float intensity;

void main(void){
	light = vec4(-1.0, -1.0, -1.0, 0.0);
	light = normalize(light);
	intensity = -dot(light, n_out);
	if(intensity < 0) intensity = 0;

	color = vec4(intensity);
}
