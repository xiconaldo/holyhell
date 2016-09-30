#version 430 core

layout (binding = 0) uniform sampler2D grass;
in vec2 text_coord;
out vec4 color;

void main(){
	color = texture(grass, text_coord*30.0f);
}