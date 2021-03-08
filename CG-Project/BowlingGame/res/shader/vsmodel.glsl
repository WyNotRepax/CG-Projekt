#version 410

uniform mat4 ModelViewProj;
uniform mat4 Model;

layout(location=0) in vec4 position;
layout(location=1) in vec4 normal;
layout(location=2) in vec2 texCoordIn;

out vec2 texCoord;
out vec3 pos;
void main(){
	gl_Position =  ModelViewProj * position;
	texCoord = texCoordIn;
	pos = (Model * position).xyz;
}