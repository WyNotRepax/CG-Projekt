#include "GameShader.h"
#include "../Logging.h"

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
	if (mViewProjChanged || mModelChanged) {
		setUniformMatrix(mModelViewProjLoc, mViewProj * mModel);
	}
	if (mModelChanged) {
		setUniformMatrix(mModelLoc, mModel);
	}
	mViewProjChanged = false;
	mModelChanged = false;
	if (mEyePosChanged) {
		setUniformVec(mEyePosLoc, mEyePos);
	}
	mEyePosChanged = false;

	setUniformId(mDifftexLoc, 0);
	LOG_CALL(glActiveTexture, GL_TEXTURE0);
	LOG_CALL(glBindTexture, GL_TEXTURE_2D, mDiffTexId);
}

void GameShader::setViewProj(const Matrix& matrix)
{
	mViewProj = matrix;
	mViewProjChanged = true;
}

void GameShader::setModel(const Matrix& matrix)
{
	mModel = matrix;
	mModelChanged = true;
}

void GameShader::setDiffTex(GLuint texId)
{
	mDiffTexId = texId;
}

void GameShader::setEyePos(const Vector& eyePos)
{
	mEyePos = eyePos;
	mEyePosChanged = true;
}

GameShader::GameShader() :
	Shader(GAME_SHADER_VS_PATH, GAME_SHADER_FS_PATH),
	mViewProjChanged(true),
	mViewProj(Matrix().identity()),
	mModelViewProjLoc(0),
	mDifftexLoc(0),
	mModel(Matrix().identity()),
	mModelLoc(0),
	mModelChanged(true),
	mEyePos(Vector(0, 0, 0)),
	mEyePosChanged(true),
	mEyePosLoc(0)
{
	mModelViewProjLoc = Shader::getUniformLocation(GAME_SHADER_MODELVIEWPROJ_UNIFORM_NAME);
	mDifftexLoc = Shader::getUniformLocation(GAME_SHADER_DIFFTEX_UNIFORM_NAME);
	mModelLoc = Shader::getUniformLocation(GAME_SHADER_MODEL_UNIFORM_NAME);
	mEyePosLoc = Shader::getUniformLocation(GAME_SHADER_EYEPOS_UNIFORM_NAME);
}
