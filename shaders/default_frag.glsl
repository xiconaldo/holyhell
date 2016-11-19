#version 430 core

layout (location = 3) uniform vec3 light = vec3(-1.0f, -1.0f, -1.0f);
layout (binding  = 0) uniform sampler2D base_texture;
layout (binding  = 2) uniform sampler2D shadow_map;

in  vec3 norm_coord;
in  vec2 text_coord;
in  vec4 shadow_coord;
out vec4 color;

vec3 local_light;
float intensity = 1.0f;
float bias = 0.005;

void main(){
	local_light = normalize(light);
	intensity = -dot(norm_coord, local_light);
	if(intensity < 0.2) intensity = 0.2;

	if ( texture(shadow_map, shadow_coord.xy).r  <  shadow_coord.r - bias){
	   intensity = 0.2f;
	}

	vec4 text_color = texture(base_texture, text_coord);
	color = vec4(text_color.xyz * pow(intensity, 2), text_color.a);
}