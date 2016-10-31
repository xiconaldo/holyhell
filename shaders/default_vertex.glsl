#version 430 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

layout (location = 0) uniform mat4 model = mat4(1.0f);
layout (location = 1) uniform mat4 view  = mat4(1.0f);
layout (location = 2) uniform mat4 proj  = mat4(1.0f);

out vec3 norm_coord;
out vec2 text_coord;

void main(){
	vec4 pos = vec4(vertex, 1.0f);

	gl_Position = proj * view * model * pos;
	
	norm_coord = normal;
	text_coord = uv;
}