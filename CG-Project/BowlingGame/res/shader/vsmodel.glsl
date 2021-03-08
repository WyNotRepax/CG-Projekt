#version 410

uniform mat4 ModelViewProj;
layout(location=0) in vec4 position;
layout(location=1) in vec4 normal;
layout(location=2) in vec2 texCoord;

out vec4 colorTest;
void main(){
	gl_Position =  ModelViewProj * position;
	colorTest = vec4(texCoord,1,1);
}