#version 430 core

layout (vertices = 4) out;
float tessLevel = 40.0f;

void main(){

	if(gl_InvocationID == 0){
		gl_TessLevelInner[0] = tessLevel;
		gl_TessLevelInner[1] = tessLevel;
		gl_TessLevelOuter[0] = tessLevel;
		gl_TessLevelOuter[1] = tessLevel;
		gl_TessLevelOuter[2] = tessLevel;
		gl_TessLevelOuter[3] = tessLevel;
	}

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
