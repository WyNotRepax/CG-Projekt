#pragma once
#include "Shader.h"
#include "../Directories.h"

#define SHADOW_SHADER_FS_PATH SHADER_DIR "/fsdepth.glsl"
#define SHADOW_SHADER_VS_PATH SHADER_DIR "/vsdepth.glsl"
#define SHADOW_SHADER_MODELVIEWPROJ_UNIFORM_NAME "ModelViewProjMat"
#define SHADOW_SHADER_MODEL_UNIFORM_NAME "ModelMat"
#define SHADOW_SHADER_EYEPOS_UNIFORM_NAME "EyePos"
class ShadowShader :
    public Shader
{
private:
	static ShadowShader* pInstance;

	Matrix mModelViewProj;
	bool mModelViewProjChanged;
	GLint mModelViewProjLoc;
public:
	ShadowShader(const ShadowShader& other) = delete;
	virtual ~ShadowShader() = default; //TODO
	static ShadowShader* GetInstance();
	bool operator=(const ShadowShader& other) = delete;
	virtual void activate() override;
	void setModelViewProj(const Matrix& matrix);
protected:
	ShadowShader();
};

