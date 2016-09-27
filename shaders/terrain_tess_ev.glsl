#version 430 core

layout(triangles, equal_spacing) in;

layout (location = 0) uniform mat4 model = mat4(1.0f);
//layout (location = 1) uniform mat4 view  = mat4(1.0f);
//layout (location = 2) uniform mat4 proj  = mat4(1.0f);
//layout (binding  = 1) uniform sampler2D height_map;

void main(){
	//vec4 pos0 = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_TessCoord.x);
	//vec4 pos1 = mix(gl_in[2].gl_Position, gl_in[3].gl_Position, gl_TessCoord.x);
	//vec4 pos  = mix(pos0, pos1, gl_TessCoord.y);

	vec4 pos =  gl_in[0].gl_Position * gl_TessCoord.x +
				gl_in[1].gl_Position * gl_TessCoord.y +
				gl_in[2].gl_Position * gl_TessCoord.z;

	//float height = texture(height_map, pos.xy).g;
	//pos.y = pos.y + height;
	gl_Position = model * pos;
}