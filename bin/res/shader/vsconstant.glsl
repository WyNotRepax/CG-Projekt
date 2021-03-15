#version 410

uniform mat4 ModelViewProj;
layout(location=0) in vec4 position;

void main(){
	gl_Position =  ModelViewProj * position;
}