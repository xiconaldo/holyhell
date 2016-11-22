#version 430 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

layout (location = 0) uniform mat4 model = mat4(1.0f);
layout (location = 1) uniform mat4 view  = mat4(1.0f);
layout (location = 2) uniform mat4 proj  = mat4(1.0f);

layout (location = 3) uniform vec3  light_dir = vec3(-1.0f, -1.0f, -1.0f);
layout (location = 4) uniform float I_light = 1.0f;
layout (location = 5) uniform float I_amb = 0.2f;
layout (location = 6) uniform vec3  k = vec3(1.0f, 1.0f, 0.0f);
layout (location = 7) uniform float desat = 0.0f;

layout (std140, binding = 0) uniform Player{
	float x;
	float z;
} player;

layout (binding  = 5) uniform sampler2D height_map;

out vec2 text_coord;
out float I_factor;

void main(){
	vec4 pos = vec4(vertex, 1.0f);

	mat4 m_model = model;
	m_model[3].y += texture(height_map, model[3].xz * 0.5f + 0.5f).a * 0.15f - 0.01;
	
	m_model[3].x -= player.x;
	m_model[3].z -= player.z;
	m_model[3].y -= texture(height_map, vec2(player.x, player.z) * 0.5f + 0.5f).a * 0.15f + 0.048f;

	gl_Position = proj * view * m_model * pos;
	text_coord = uv;

	vec3 my_normal = normalize((transpose(inverse(m_model)) * vec4(normal, 1.0f)).xyz);

	float NL = max(-dot(my_normal, normalize(light_dir)), 0.0f);
	float RV = 1.0f;
	float n = 1.0f;
	I_factor = I_amb * k.x + I_light * (k.y * NL + k.z * pow(RV, n));
}