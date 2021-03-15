#include "GameShader.h"
#include "../Logging.h"
#include "../game/GameObject.h"
#include "ShadowShader.h"

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
		setUniformInt(mLightCountLoc, mLightCount);
		for (unsigned int lightIndex = 0; lightIndex < mLightCount; lightIndex++) {
			setUniformInt(mLightLocs[lightIndex].typeLoc, mLights[lightIndex].Type);
			setUniformVec(mLightLocs[lightIndex].colorLoc, mLights[lightIndex].Color);
			setUniformVec(mLightLocs[lightIndex].positionLoc, mLights[lightIndex].Position);
			setUniformVec(mLightLocs[lightIndex].directionLoc, mLights[lightIndex].Direction);
			setUniformVec(mLightLocs[lightIndex].attenuationLoc, mLights[lightIndex].Attenuation);
			setUniformVec(mLightLocs[lightIndex].spotRadiusLoc, mLights[lightIndex].SpotRadius);
			if (mLights[lightIndex].Type == Light::SPOT) {
				LOG_CALL(glActiveTexture, GL_TEXTURE0 + lightIndex);
				LOG_CALL(glBindTexture, GL_TEXTURE_2D, mShadowMapTex[lightIndex]);
				setUniformInt(mLightLocs[lightIndex].texLoc, lightIndex);
				Matrix matrix = mLights[lightIndex].getProj() * mLights[lightIndex].getView();
				//LOG("Setting Matrix: %s", ((std::string)matrix).c_str());
				setUniformMatrix(mLightLocs[lightIndex].matLoc, matrix);
			}
		}
	}
	mLightsChanged = false;
	setUniformInt(mDifftexLoc, 31);
	LOG_CALL(glActiveTexture, GL_TEXTURE31);
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

void GameShader::generateShadows(std::vector<GameObject*> gameObjects)
{
	glClearColor(1.0f, 0.0, 0.0f, 1.0f);
	glCullFace(GL_FRONT);
	// Save Old Viewport Data
	GLint previousViewPort[4];
	LOG_CALL(glGetIntegerv, GL_VIEWPORT, previousViewPort);
	LOG_CALL(glViewport, 0, 0, GAME_SHADER_LIGHTS_SHADOW_MAP_TEXTURE_SIZE, GAME_SHADER_LIGHTS_SHADOW_MAP_TEXTURE_SIZE);
	
	LOG_CALL(glBindFramebuffer, GL_FRAMEBUFFER, mFrameBufferId);
	for (unsigned int i = 0; i < mLightCount; i++) {
		if (mLights[i].Type == Light::SPOT) {
			//LOG("Drawing Depth Texture for %d\n", i);
			// This Should draw to the selected Texture but somehow doesn't
			LOG_CALL(glFramebufferTexture2D, GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mShadowMapTex[i], 0);
			LOG_CALL(glClear, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			mShadowCam.setView(mLights[i].getView());
			mShadowCam.setProj(mLights[i].getProj());
			
			for (GameObject* pGameObject : gameObjects) {
				pGameObject->draw(&mShadowCam, ShadowShader::GetInstance());
			}
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
		}
	}
	LOG_CALL(glBindFramebuffer, GL_FRAMEBUFFER, 0);
	glClearColor(0, 0, 0, 1);
	glCullFace(GL_BACK);
	LOG_CALL(glViewport, previousViewPort[0], previousViewPort[1], previousViewPort[2], previousViewPort[3]);
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
	mAmbColLoc(0),
	mShadowCam()
{
	mModelViewProjLoc = Shader::getUniformLocation(GAME_SHADER_MODELVIEWPROJ_UNIFORM_NAME);
	mDifftexLoc = Shader::getUniformLocation(GAME_SHADER_DIFFTEX_UNIFORM_NAME);
	mModelLoc = Shader::getUniformLocation(GAME_SHADER_MODEL_UNIFORM_NAME);
	mEyePosLoc = Shader::getUniformLocation(GAME_SHADER_EYEPOS_UNIFORM_NAME);
	mDiffColLoc = Shader::getUniformLocation(GAME_SHADER_DIFFCOL_UNIFORM_NAME);
	mSpecColLoc = Shader::getUniformLocation(GAME_SHADER_SPECCOL_UNIFORM_NAME);
	mSpecExpLoc = Shader::getUniformLocation(GAME_SHADER_SPECEXP_UNIFORM_NAME);
	mAmbColLoc = Shader::getUniformLocation(GAME_SHADER_AMBCOL_UNIFORM_NAME);
	LOG_CALL(glGenFramebuffers, 1, &mFrameBufferId);

	LOG_CALL(glGenTextures, GAME_SHADER_MAX_LIGHTS, mShadowMapTex);

	GLuint depth;
	LOG_CALL(glGenRenderbuffers, 1, &depth);
	LOG_CALL(glBindRenderbuffer, GL_RENDERBUFFER, depth);
	LOG_CALL(glRenderbufferStorage, GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, GAME_SHADER_LIGHTS_SHADOW_MAP_TEXTURE_SIZE, GAME_SHADER_LIGHTS_SHADOW_MAP_TEXTURE_SIZE);
	LOG_CALL(glBindRenderbuffer, GL_RENDERBUFFER, 0);
	LOG_CALL(glBindFramebuffer, GL_FRAMEBUFFER, mFrameBufferId);
	LOG_CALL(glFramebufferRenderbuffer, GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth);
	LOG_CALL(glBindFramebuffer, GL_FRAMEBUFFER, 0);

	for (unsigned int i = 0; i < GAME_SHADER_MAX_LIGHTS; i++) {
		std::string prefix = GAME_SHADER_LIGHTS_UNIFORM_NAME"[" + std::to_string(i) + "].";
		mLightLocs[i].typeLoc = Shader::getUniformLocation(prefix + GAME_SHADER_LIGHTS_TYPE_UNIFORM_NAME);
		mLightLocs[i].colorLoc = Shader::getUniformLocation(prefix + GAME_SHADER_LIGHTS_COLOR_UNIFORM_NAME);
		mLightLocs[i].attenuationLoc = Shader::getUniformLocation(prefix + GAME_SHADER_LIGHTS_ATTENUATION_UNIFORM_NAME);
		mLightLocs[i].spotRadiusLoc = Shader::getUniformLocation(prefix + GAME_SHADER_LIGHTS_SPOTRADIUS_UNIFORM_NAME);
		mLightLocs[i].positionLoc = Shader::getUniformLocation(prefix + GAME_SHADER_LIGHTS_POSITION_UNIFORM_NAME);
		mLightLocs[i].directionLoc = Shader::getUniformLocation(prefix + GAME_SHADER_LIGHTS_DIRECTION_UNIFORM_NAME);
		mLightLocs[i].texLoc = Shader::getUniformLocation(prefix + GAME_SHADER_LIGHTS_DEPTHTEX_UNIFORM_NAME);
		mLightLocs[i].matLoc = Shader::getUniformLocation(prefix + GAME_SHADER_LIGHTS_MATRIX_UNIFORM_NAME);
		LOG_CALL(glBindTexture, GL_TEXTURE_2D, mShadowMapTex[i]);
		LOG_CALL(glTexImage2D, GL_TEXTURE_2D, 0, GL_R32F, GAME_SHADER_LIGHTS_SHADOW_MAP_TEXTURE_SIZE, GAME_SHADER_LIGHTS_SHADOW_MAP_TEXTURE_SIZE, 0, GL_RED, GL_FLOAT, nullptr);
	}
	LOG_CALL(glBindTexture, GL_TEXTURE_2D, 0);
	mLightCountLoc = Shader::getUniformLocation(GAME_SHADER_LIGHTCOUNT_UNIFORM_NAME);
}
