#version 400

uniform vec3 Color;

in vec4 colorTest;
out vec4 FragColor;

void main(){
	FragColor = vec4(Color,1.0);
	FragColor = abs(colorTest);
}