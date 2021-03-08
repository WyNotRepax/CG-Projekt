#include "GameShader.h"

GameShader* GameShader::pInstance = nullptr;

GameShader* GameShader::GetInstance()
{
	if (GameShader::pInstance == nullptr) {
		GameShader::pInstance = new GameShader();
	}
	return GameShader::pInstance;
}

void GameShader::activate()
{
	Shader::activate();
	if (mModelViewProjChanged) {
		setUniformMatrix(mModelViewProjLoc, mModelViewProj);
	}
}

void GameShader::setModelViewProj(const Matrix& matrix)
{
	mModelViewProj = matrix;
	mModelViewProjChanged = true;
}

GameShader::GameShader() :
	Shader(GAME_SHADER_VS_PATH, GAME_SHADER_FS_PATH),
	mModelViewProjChanged(true),
	mModelViewProj(Matrix().identity()),
	mModelViewProjLoc(0)
{
	mModelViewProjLoc = Shader::getUniformLocation(GAME_SHADER_MODELVIEWPROJ_UNIFORM_NAME);
}
