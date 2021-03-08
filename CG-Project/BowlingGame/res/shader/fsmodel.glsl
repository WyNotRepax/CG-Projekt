#version 400

uniform vec3 Color;

in vec2 texCoord;

out vec4 FragColor;

uniform vec3 DiffuseColor;
uniform vec3 SpecularColor;
uniform vec3 AmbientColor;
uniform float SpecularExp;
uniform sampler2D DiffTex;
uniform vec3 EyePos;

void main(){
	FragColor = texture(DiffTex,texCoord);
}