#version 430 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

layout (location = 0) uniform mat4 model = mat4(1.0f);
layout (location = 1) uniform mat4 view  = mat4(1.0f);
layout (location = 2) uniform mat4 proj  = mat4(1.0f);

layout (location = 8) uniform mat4 shadow_view  = mat4(1.0f);
layout (location = 9) uniform mat4 shadow_proj  = mat4(1.0f);
layout (location = 10) uniform mat4 shadow_bias  = mat4(1.0f);

layout (std140, binding = 0) uniform Player{
	float x;
	float z;
} player;

layout (binding  = 5) uniform sampler2D height_map;

out vec3 norm_coord;
out vec2 text_coord;
out vec4 shadow_coord;

void main(){
	vec4 pos = vec4(vertex, 1.0f);

	mat4 m_model = model;
	m_model[3].y += texture(height_map, model[3].xz * 0.5f + 0.5f).a * 0.15f;
	
	// Same, but with the light's view matrix
	shadow_coord = shadow_proj * shadow_view * m_model * pos;
	shadow_coord /= shadow_coord.w;
	shadow_coord = shadow_bias * shadow_coord;

	m_model[3].x -= player.x;
	m_model[3].z -= player.z;
	m_model[3].y -= texture(height_map, vec2(player.x, player.z) * 0.5f + 0.5f).a * 0.15f + 0.038f;

	gl_Position = proj * view * m_model * pos;

	norm_coord = normal;
	text_coord = uv;
}