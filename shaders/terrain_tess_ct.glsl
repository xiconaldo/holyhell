#version 430 core

layout (vertices = 3) out;

void main(){

	if(gl_InvocationID == 0){
		gl_TessLevelInner[0] = 5.0f;
		//gl_TessLevelInner[1] = 5.0f;
		gl_TessLevelOuter[0] = 2.0f;
		gl_TessLevelOuter[1] = 2.0f;
		gl_TessLevelOuter[2] = 2.0f;
		//gl_TessLevelOuter[3] = 5.0f;
	}

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
