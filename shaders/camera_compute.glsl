#version 430 core

layout (local_size_x = 1) in;

layout (location = 0) uniform mat4 model = mat4(1.0f);
layout (std430, binding = 0) buffer camera{
	mat4 view;
} Camera;

layout (binding  = 5) uniform sampler2D height_map;

void main(){
	mat4 translation = mat4(1.0f);
	translation[3].y -= texture(height_map, model[3].xz * 0.5f + 0.5f).a * 0.15f;
	Camera.view = Camera.view * translation;

	barrier();
}