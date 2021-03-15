#version 400
in vec4 Position;
out vec4 FragColor;

void main()
{
	vec4 Pos = Position / Position.w;
	FragColor = vec4(Pos.z,0,0,1);
}
