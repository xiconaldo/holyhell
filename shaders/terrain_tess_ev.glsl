#version 430 core

layout(quads, equal_spacing, ccw) in;

layout (location = 0) uniform mat4 model = mat4(1.0f);
layout (location = 1) uniform mat4 view  = mat4(1.0f);
layout (location = 2) uniform mat4 proj  = mat4(1.0f);
layout (location = 3) uniform vec3 light = vec3(-5.0f, -1.0f, 0.0f);
layout (location = 4) uniform mat4 rev_model = mat4(1.0f);

layout (std140, binding = 0) uniform Player{
	float x;
	float z;
} player;

layout (binding  = 5) uniform sampler2D height_map;

out vec2 text_coord;
out float intensity;
//out vec4 light;

void main(){
	vec4 pos0 = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_TessCoord.x);
	vec4 pos1 = mix(gl_in[3].gl_Position, gl_in[2].gl_Position, gl_TessCoord.x);
	vec4 pos  = mix(pos0, pos1, gl_TessCoord.y);

	text_coord = gl_TessCoord.xy;
	float height = texture(height_map, text_coord).a;

	// MODEL SIMULADA AQUI
	pos.y += height * 0.15f;
	pos.x -= player.x;
	pos.z -= player.z;
	pos.y -= texture(height_map, vec2(player.x, player.z) * 0.5f + 0.5f).a * 0.15f + 0.038f;

	// ILUMINAÇÃO CALCULADA AQUI
	vec3 normal = texture(height_map, text_coord).rgb;
	normal = normalize(normal);
	vec3 local_light = normalize(light);

	intensity = -dot(normal, local_light);
	if(intensity < 0.4f) intensity = 0.4f;

	gl_Position = proj * view * pos;
}