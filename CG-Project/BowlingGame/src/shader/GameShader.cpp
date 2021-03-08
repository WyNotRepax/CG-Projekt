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

	if (mDiffColChanged) {
		setUniformVec(mDiffColLoc, mDiffCol);
	}
	mDiffColChanged = false;

	if (mSpecColChanged) {
		setUniformVec(mSpecColLoc, mSpecCol);
	}
	mSpecColChanged = false;

	if (mAmbColChanged) {
		setUniformVec(mAmbColLoc, mAmbCol);
	}
	mAmbColChanged = false;

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

void GameShader::setDiffCol(const Vector& diffCol)
{
	mDiffCol = diffCol;
	mDiffColChanged = true;
}

void GameShader::setSpecCol(const Vector& specCol)
{
	mSpecCol = specCol;
	mSpecColChanged = true;
}

void GameShader::setAmbCol(const Vector& ambCol)
{
	mAmbCol = ambCol;
	mAmbColChanged = true;
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
	mEyePosLoc(0),
	mDiffCol(Vector(0, 0, 0)),
	mDiffColChanged(true),
	mDiffColLoc(0),
	mSpecCol(Vector(0, 0, 0)),
	mSpecColChanged(true),
	mSpecColLoc(0),
	mAmbCol(Vector(0, 0, 0)),
	mAmbColChanged(true),
	mAmbColLoc(0)
{
	mModelViewProjLoc = Shader::getUniformLocation(GAME_SHADER_MODELVIEWPROJ_UNIFORM_NAME);
	mDifftexLoc = Shader::getUniformLocation(GAME_SHADER_DIFFTEX_UNIFORM_NAME);
	mModelLoc = Shader::getUniformLocation(GAME_SHADER_MODEL_UNIFORM_NAME);
	mEyePosLoc = Shader::getUniformLocation(GAME_SHADER_EYEPOS_UNIFORM_NAME);
	mDiffColLoc = Shader::getUniformLocation(GAME_SHADER_DIFFCOL_UNIFORM_NAME);
	mSpecColLoc = Shader::getUniformLocation(GAME_SHADER_SPECCOL_UNIFORM_NAME);
	mAmbColLoc = Shader::getUniformLocation(GAME_SHADER_AMBCOL_UNIFORM_NAME);
}
