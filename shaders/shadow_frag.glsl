#version 430 core

// Ouput data
layout(location = 0) out float fragmentdepth;
//out vec4 color;

void main(){
    // Not really needed, OpenGL does it anyway
    //fragmentdepth = gl_FragCoord.z;
    //float uho = gl_FragCoord.z;
    //uho = pow(uho, 200);
    //color = vec4(uho, uho, uho, 1.0f);
}