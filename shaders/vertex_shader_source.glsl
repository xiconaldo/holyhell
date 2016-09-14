#version 430 core

layout (location = 0) in vec4 vertex; 
layout (location = 1) in vec4 normal;

out vec4 n_out;
 
void main(void){
	
	gl_Position = vec4(vertex.xyz * 0.1f, vertex.w) + vec4(0,-1,0,0);
	n_out = normalize(normal);
}
