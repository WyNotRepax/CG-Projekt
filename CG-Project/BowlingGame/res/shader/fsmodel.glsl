#version 400

uniform vec3 Color;

in vec2 texCoord;
in vec3 pos;
in vec3 normal;
in vec4 ndc;

out vec4 FragColor;

uniform vec3 DiffuseColor;
uniform vec3 SpecularColor;
uniform vec3 AmbientColor;
uniform float SpecularExp;
uniform sampler2D DiffTex;
uniform vec3 EyePos;

const int MAX_LIGHTS=31;
struct Light
{
	int Type;
	vec3 Color;
	vec3 Position;
	vec3 Direction;
	vec3 Attenuation;
	vec3 SpotRadius;
	sampler2D ShadowMapTexture;
	mat4x4 Mat;
};

uniform int LightCount;
uniform Light Lights[MAX_LIGHTS];

float sat(in float f){
	return clamp(f, 0.0, 1.0);
}

void main(){
	vec3 Test = ndc.xyz / ndc.w;
	vec2 screenKoord = (Test.xy + vec2(1,1)) * 0.5;

	vec4 TexColor = texture(DiffTex, texCoord);
	vec3 N = normalize(normal);
	vec3 E = normalize(EyePos - pos);

	vec3 TotalDiff = vec3(0,0,0);
	vec3 TotalSpec = vec3(0,0,0);

	for(unsigned int lightIndex = 0; lightIndex < LightCount; lightIndex++){
		vec3 L;
		float I;
		if(Lights[lightIndex].Type == 1){ 
			L = -Lights[lightIndex].Direction;
			I = 1;
		}
		else{
			L = normalize(Lights[lightIndex].Position - pos);
			float dist = distance(Lights[lightIndex].Position,pos);
			I = 1.0 / (Lights[lightIndex].Attenuation.x + Lights[lightIndex].Attenuation.y * dist + Lights[lightIndex].Attenuation.z * dist * dist);
			if(Lights[lightIndex].Type == 2){
                float ang = abs(acos(dot(normalize(Lights[lightIndex].Direction), -L)));
                I = I * sat((ang-Lights[lightIndex].SpotRadius.y)/(Lights[lightIndex].SpotRadius.x-Lights[lightIndex].SpotRadius.y));  //*(1/(dist*dist))
			}
		}
		vec3 R = reflect(-L,N);
		vec3 Diff = Lights[lightIndex].Color * DiffuseColor * TexColor.rgb * sat(dot(N,L));
		vec3 Spec = Lights[lightIndex].Color * SpecularColor * pow(sat(dot(R,E)), SpecularExp);
		if(Lights[lightIndex].Type == 2){
			vec4 PosSM = Lights[lightIndex].Mat * vec4(pos, 1);
			PosSM.xyz /= PosSM.w;
			PosSM.xy = PosSM.xy * 0.5 + 0.5;
			float DepthSM = texture(Lights[lightIndex].ShadowMapTexture, PosSM.xy).r;
			if (PosSM.x > 1 || PosSM.x < 0 || PosSM.y > 1 || PosSM.y < 0){
				TotalDiff += Diff * I;
				TotalSpec += Spec * I;
				continue;
			}
			else if (DepthSM > PosSM.z || true){
				// Make the Check always succeed since Shadow Map Generation see GameShader.cpp Line 143
				TotalDiff += Diff * I;
				TotalSpec += Spec * I;
			}
		}else{
			TotalDiff += Diff * I;
			TotalSpec += Spec * I;
		}
	}
	vec3 Amb = AmbientColor * TexColor.rgb;
	FragColor = vec4(TotalDiff + TotalSpec + Amb,1);
	

}