#version 430 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

layout (location = 0) uniform mat4 model = mat4(1.0f);
layout (location = 1) uniform mat4 view  = mat4(1.0f);
layout (location = 2) uniform mat4 proj  = mat4(1.0f);
layout (binding  = 1) uniform sampler2D height;

out vec3 norm_coord;
out vec2 text_coord;

mat4 grass_translation(void){

	vec3 coord;

	// coord.x = float(gl_InstanceID & 0x00ff) + ((gl_InstanceID * 17 + 191) % 200 - 100) / 20.0f;
	// coord.z = float((gl_InstanceID & 0xff00) >> 8) + ((gl_InstanceID * 29 + 383) % 200 - 100) / 20.0f;
	// //coord.xz = fma(coord.xz, vec2(0.0078125), vec2(-1.0f));
	// coord.xz = coord.xz / 128.0f - vec2(1.0f);

	coord.x = float(gl_InstanceID & 0x01ff) + ((gl_InstanceID * 3 + 191) % 200 - 100) / 20.0f;
	coord.z = float((gl_InstanceID & 0x3fe00) >> 9) + ((gl_InstanceID * 29 + 383) % 200 - 100) / 20.0f;
	coord.xz = coord.xz / 256.0f - vec2(1.0f);
	coord.xz = clamp(coord.xz, vec2(-0.99f), vec2(0.99f));

	coord.y = texture(height, coord.xz * 0.5f + 0.5f).a * 0.15f;
	
	mat4 trans = mat4(1.0f);
	trans[3] = vec4(coord, 1.0f);

	return trans;
}

mat4 grass_rotation(){
	mat4 roty = mat4(1.0f);

	float ty = ((gl_InstanceID * 17 + 191) % 360) * 3.1415f / 180.0f;
	roty[0].x = cos(ty);
	roty[0].z = sin(ty);
	roty[2].x = -sin(ty);
	roty[2].z = cos(ty);

	mat4 rotx = mat4(1.0f);

	float tx = (((gl_InstanceID * 23 + 179) % 80) - 40) * 3.1415f / 180.0f;
	roty[1].y = cos(tx);
	roty[1].z = sin(tx);
	roty[2].y = -sin(tx);
	roty[2].z = cos(tx);

	return roty * rotx;
}

mat4 grass_scale(){
	float s = ((gl_InstanceID * 17 + 191) % 200 + 100) / 80.0f;
	mat4 scale = mat4(1.0f);
	scale[1].y = s;

	return scale;
}

void main(){

	mat4 trans = grass_translation();
	mat4 rot = grass_rotation();
	mat4 scale = grass_scale();
	
	mat4 m_model = trans * rot * scale * model;

	gl_Position = proj * view * m_model * vec4(vertex, 1.0f);
	norm_coord = normal;
	text_coord = uv;
}