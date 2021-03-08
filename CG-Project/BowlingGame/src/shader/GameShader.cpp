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

	if (mSpecExpChanged) {
		setUniformFloat(mSpecExpLoc, mSpecExp);
	}
	mSpecExpChanged = false;

	if (mAmbColChanged) {
		setUniformVec(mAmbColLoc, mAmbCol);
	}
	mAmbColChanged = false;

	if (mLightsChanged) {
			for (unsigned int lightIndex = 0; lightIndex < mLightCount; lightIndex++) {
				setUniformInt(mLightLocs->typeLoc, mLights->Type);
				setUniformVec(mLightLocs->colorLoc, mLights->Color);
				setUniformVec(mLightLocs->positionLoc, mLights->Position);
				setUniformVec(mLightLocs->directionLoc, mLights->Direction);
				setUniformVec(mLightLocs->attenuationLoc, mLights->Attenuation);
				setUniformVec(mLightLocs->spotRadiusLoc, mLights->SpotRadius);
			}
	}
	mLightsChanged = false;
	setUniformInt(mDifftexLoc, 0);
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

void GameShader::setSpecExp(const float& f)
{
	mSpecExp = f;
	mSpecExpChanged = true;
}

void GameShader::setAmbCol(const Vector& ambCol)
{
	mAmbCol = ambCol;
	mAmbColChanged = true;
}

void GameShader::addLight(const Light& light)
{
	mLights[mLightCount++] = light;
	
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
	mSpecExpLoc = Shader::getUniformLocation(GAME_SHADER_SPECEXP_UNIFORM_NAME);
	mAmbColLoc = Shader::getUniformLocation(GAME_SHADER_AMBCOL_UNIFORM_NAME);
	for (unsigned int i = 0; i < GAME_SHADER_MAX_LIGHTS; i++) {
		std::string prefix = GAME_SHADER_LIGHTS_UNIFORM_NAME"[" + std::to_string(i) + "].";
		mLightLocs[i].typeLoc = Shader::getUniformLocation(prefix + GAME_SHADER_LIGHTS_TYPE_UNIFORM_NAME);
		mLightLocs[i].colorLoc = Shader::getUniformLocation(prefix + GAME_SHADER_LIGHTS_COLOR_UNIFORM_NAME);
		mLightLocs[i].attenuationLoc = Shader::getUniformLocation(prefix + GAME_SHADER_LIGHTS_ATTENUATION_UNIFORM_NAME);
		mLightLocs[i].spotRadiusLoc = Shader::getUniformLocation(prefix + GAME_SHADER_LIGHTS_SPOTRADIUS_UNIFORM_NAME);
		mLightLocs[i].positionLoc = Shader::getUniformLocation(prefix + GAME_SHADER_LIGHTS_POSITION_UNIFORM_NAME);
		mLightLocs[i].directionLoc = Shader::getUniformLocation(prefix + GAME_SHADER_LIGHTS_DIRECTION_UNIFORM_NAME);
	}
}
