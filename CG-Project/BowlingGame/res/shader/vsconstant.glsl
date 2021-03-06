#version 410

uniform mat4 ModelViewProj;
layout(location=0) in vec4 position;
layout(location=1) in vec4 normal;

out vec4 colorTest;
void main(){
	gl_Position =  ModelViewProj * position;
	colorTest = vec4(normal.xyz,1);
}