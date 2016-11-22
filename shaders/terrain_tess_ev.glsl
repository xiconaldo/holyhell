#version 430 core

layout(quads, equal_spacing, ccw) in;

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
	vec4 pos0 = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_TessCoord.x);
	vec4 pos1 = mix(gl_in[3].gl_Position, gl_in[2].gl_Position, gl_TessCoord.x);
	vec4 pos  = mix(pos0, pos1, gl_TessCoord.y);

	text_coord = gl_TessCoord.xy;
	float height = texture(height_map, text_coord).a;

	mat4 m_model = model;
	m_model[3].y += height * 0.15f;
	m_model[3].x -= player.x;
	m_model[3].z -= player.z;
	m_model[3].y -= texture(height_map, vec2(player.x, player.z) * 0.5f + 0.5f).a * 0.15f + 0.048f;

	gl_Position = proj * view * m_model * pos;

	vec3 normal = texture(height_map, text_coord).rgb;
	normal = normal*2 - vec3(1.0f);

	vec3 my_normal = normalize((transpose(inverse(m_model)) * vec4(normal.xzy, 0.0f)).xyz);

	float NL = max(-dot(my_normal, normalize(light_dir)), 0.0f);
	float RV = 1.0f;
	float n = 1.0f;
	I_factor = I_amb * k.x + I_light * (k.y * NL + k.z * pow(RV, n));
}