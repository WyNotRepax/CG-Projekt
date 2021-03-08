#include "ConstantShader.h"


ConstantShader* ConstantShader::pInstance = nullptr;

ConstantShader::ConstantShader() :
	Shader(CONSTANT_SHADER_VS_PATH, CONSTANT_SHADER_FS_PATH),
	mR(0.0f), mG(0.0f), mB(0.0f), mColorChanged(true), mModelViewProjChanged(true) {
	mColorLoc = getUniformLocation(CONSTANT_SHADER_COLOR_UNIFORM_NAME);
	mModelViewProjLoc = getUniformLocation(CONSTANT_SHADER_MODELVIEWPROJ_UNIFORM_NAME);
}

ConstantShader::~ConstantShader()
{
}

ConstantShader* ConstantShader::GetInstance()
{
	if (pInstance == nullptr) {
		pInstance = new ConstantShader();
	}
	return pInstance;
}

void ConstantShader::activate()
{
	Shader::activate();
	if (mColorChanged) {
		setUniformVec(mColorLoc,mR, mG, mB);
	}
	if (mModelViewProjChanged) {
		setUniformMatrix(mModelViewProjLoc,mModelViewProj);
	}
}

void ConstantShader::setColor(float r, float g, float b)
{
	mR = r;
	mG = g;
	mB = b;
	mColorChanged = true;
}

void ConstantShader::setModelViewProj(const Matrix& matrix) {
	mModelViewProj = matrix;
	mModelViewProjChanged = true;
}


