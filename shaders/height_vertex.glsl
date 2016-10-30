#version 430 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;

layout (location = 0) uniform mat4 model = mat4(1.0f);
layout (location = 1) uniform mat4 view  = mat4(1.0f);
layout (location = 2) uniform mat4 proj  = mat4(1.0f);

layout (binding  = 1) uniform sampler2D height;

out vec3 norm_c;

void main(){
	vec4 pos = vec4(vertex, 1.0f);

	mat4 m_model = model;
	m_model[3].y += texture(height, vec2(model[3].x, model[3].z) * 0.5f + 0.5f).a * 0.15f;

	gl_Position = proj * view * m_model * pos;
	norm_c = normal;
}