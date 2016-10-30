#version 430 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;

layout (location = 0) uniform mat4 model = mat4(1.0f);
layout (location = 1) uniform mat4 view  = mat4(1.0f);
layout (location = 2) uniform mat4 proj  = mat4(1.0f);
layout (binding  = 1) uniform sampler2D height;

out vec3 norm_c;

vec4 grass_position(void){

	vec3 coord;

	coord.x = float(gl_InstanceID & 0x00ff);
	coord.z = float((gl_InstanceID & 0xff00) >> 8);
	coord.xz = fma(coord.xz, vec2(0.0078125), vec2(-1.0f));

	coord.y = texture(height, vec2(coord.x, -coord.z) * 0.5f + 0.5f).a * 0.4f;
	
	return vec4(coord, 1.0f);

}

void main(){
	mat4 m_model = model;

	float t = float(gl_InstanceID) * 3.1415 / 180.0;
	//t = 0.0f;
	mat4 rot;
	rot[0].x = cos(t);
	rot[0].z = sin(t);
	rot[2].x = -sin(t);
	rot[2].z = cos(t);
	//m_model = m_model * rot;
	m_model[3] = grass_position();

	gl_Position = proj * view * m_model * vec4(vertex, 1.0f);
	norm_c = normal;
}