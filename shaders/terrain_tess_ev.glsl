#version 430 core

layout(quads, equal_spacing, ccw) in;

layout (location = 0) uniform mat4 model = mat4(1.0f);
layout (location = 1) uniform mat4 view  = mat4(1.0f);
layout (location = 2) uniform mat4 proj  = mat4(1.0f);

layout (std140, binding = 0) uniform Player{
	float x;
	float z;
} player;

layout (binding  = 5) uniform sampler2D height_map;

out vec2 text_coord;
out vec4 light;

void main(){
	vec4 pos0 = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_TessCoord.x);
	vec4 pos1 = mix(gl_in[3].gl_Position, gl_in[2].gl_Position, gl_TessCoord.x);
	vec4 pos  = mix(pos0, pos1, gl_TessCoord.y);

	text_coord = gl_TessCoord.xy;
	float height = texture(height_map, text_coord).a;
	pos.y += height * 0.15f;

	mat4 m_view = mat4(1.0f);
	m_view[3].x = -player.x;
	m_view[3].z = -player.z;
	m_view[3].y = -texture(height_map, -m_view[3].xz * 0.5f + 0.5f).a * 0.15f - 0.038f; 

	gl_Position = proj * view * m_view * model * pos;
}