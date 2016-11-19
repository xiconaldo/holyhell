#version 430 core

layout (binding  = 2) uniform sampler2D depth;
// Ouput data
out vec4 color;
in vec2 coord;

void main(){
    // Not really needed, OpenGL does it anyway
    float merda = vec4(texture(depth, coord)).r;
    
  	color = vec4(merda, merda, merda, 1.0f);
   //color = vec4(lixo.x, lixo.y, 0.0f, 1.0f);
   //color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}