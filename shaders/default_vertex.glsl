#version 430 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;

layout (location = 0) uniform mat4 model = mat4(1.0f);
layout (location = 1) uniform mat4 view  = mat4(1.0f);
layout (location = 2) uniform mat4 proj  = mat4(1.0f);

out vec3 norm_c;

void main(){
	gl_Position = proj * view * model * vec4(vertex * 0.5f, 1.0f);
	norm_c = normal;
}