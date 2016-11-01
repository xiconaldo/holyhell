#version 430 core

layout (binding = 0) uniform sampler2D object_texture;

in vec3 norm_coord;
in vec2 text_coord;
out vec4 color;

vec3 light = vec3(-1.0f, -1.0f, -1.0f);
float intensity;

void main(){
	light = normalize(light);
	intensity = -dot(norm_coord, light);
	if(intensity < 0.4) intensity = 0.4;
	
	//color = texture(object_texture, vec2(text_coord.x, 1.0f - text_coord.y)) * pow(intensity, 2);
	color = texture(object_texture, text_coord) * pow(intensity, 2);
}