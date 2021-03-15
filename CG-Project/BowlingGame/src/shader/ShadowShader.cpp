#include "ShadowShader.h"
#include "../Logging.h"

ShadowShader* ShadowShader::pInstance = nullptr;


ShadowShader* ShadowShader::GetInstance()
{
	if (ShadowShader::pInstance == nullptr) {
		ShadowShader::pInstance = new ShadowShader();
	}
	return ShadowShader::pInstance;
}
void ShadowShader::activate()
{
	Shader::activate();
	if (mModelViewProjChanged) {
		setUniformMatrix(mModelViewProjLoc, mModelViewProj);
	}
	mModelViewProjChanged = false;
}


void ShadowShader::setModelViewProj(const Matrix& matrix)
{
	mModelViewProj = matrix;
	mModelViewProjChanged = true;
}


ShadowShader::ShadowShader() :
	Shader(SHADOW_SHADER_VS_PATH, SHADOW_SHADER_FS_PATH),
	mModelViewProjChanged(true),
	mModelViewProj(Matrix().identity()),
	mModelViewProjLoc(0)
{
	mModelViewProjLoc = Shader::getUniformLocation(SHADOW_SHADER_MODELVIEWPROJ_UNIFORM_NAME);
}
