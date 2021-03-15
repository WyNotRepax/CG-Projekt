#version 410

uniform mat4 ModelViewProj;
uniform mat4 Model;

layout(location=0) in vec4 positionIn;
layout(location=1) in vec4 normalIn;
layout(location=2) in vec2 texCoordIn;

out vec2 texCoord;
out vec3 pos;
out vec4 ndc;
out vec3 normal;

void main(){
	gl_Position =  ModelViewProj * positionIn;
	ndc = ModelViewProj * positionIn;
	texCoord = texCoordIn;
	pos = (Model * positionIn).xyz;
	normal = (Model * vec4(normalIn.xyz,0)).xyz;
}