#version 430 core

layout (location = 3) uniform vec3 light = vec3(0.0f, 0.0f, -1.0f);

layout (binding  = 0) uniform sampler2D base_texture;
layout (binding  = 2) uniform sampler2D shadow_map;
layout (binding  = 5) uniform sampler2D height_map;

in 	vec2 text_coord;
in  vec4 shadow_coord;
out vec4 color;

float bias = 0.005;

void main(){
	vec3 normal = texture(height_map, text_coord).rgb;

	vec3 local_light = normalize(light);
	normal = normalize(normal);

	float intensity = -dot(normal, local_light);
	if(intensity < 0.4f) intensity = 0.4f;

	if ( texture(shadow_map, shadow_coord.xy).z  <  shadow_coord.z - bias){
	   intensity *= 0.5f;
	}	

	vec4 text_color = texture(base_texture, text_coord*50.0f);
	color = vec4(text_color.xyz * pow(intensity, 2), text_color.a);

	//float uho = gl_FragCoord.z;
	//uho = pow(uho, 200);
	//color = vec4(uho, uho, uho, 1.0f);
}