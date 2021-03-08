#version 400

uniform vec3 Color;

in vec2 texCoord;
in vec3 pos;
in vec3 normal;

out vec4 FragColor;

uniform vec3 DiffuseColor;
uniform vec3 SpecularColor;
uniform vec3 AmbientColor;
uniform float SpecularExp;
uniform sampler2D DiffTex;
uniform vec3 EyePos;

const int MAX_LIGHTS=32;
struct Light
{
	int Type;
	vec3 Color;
	vec3 Position;
	vec3 Direction;
	vec3 Attenuation;
	vec3 SpotRadius;
};

uniform int LightCount;
uniform Light Lights[MAX_LIGHTS];

float sat(in float f){
	return clamp(f, 0.0, 1.0);
}

void main(){
	vec3 N = normalize(normal);
	vec3 L = normalize(Lights[0].Position-pos);
	vec3 E = normalize(EyePos - pos);
	vec3 R = reflect(-L,N);
	
	vec4 TexColor = texture(DiffTex, texCoord);
	vec3 Diff = DiffuseColor * TexColor.rgb * sat(dot(N,L));
	vec3 Spec = Lights[0].Color * SpecularColor * pow(sat(dot(R,E)), SpecularExp);
	vec3 Amb = AmbientColor * TexColor.rgb;
	vec3 DiffuseColor2 = vec3(1,1,1) ;
	FragColor = vec4(Diff + Spec + Amb,1);
	

}