#pragma once
#include "Shader.h"
#include "../Directories.h"
#include "Light.h"

#define GAME_SHADER_MAX_LIGHTS 32
#define GAME_SHADER_LIGHTS_UNIFORM_NAME "Lights"
#define GAME_SHADER_LIGHTS_TYPE_UNIFORM_NAME "Type"
#define GAME_SHADER_LIGHTS_COLOR_UNIFORM_NAME "Color"
#define GAME_SHADER_LIGHTS_POSITION_UNIFORM_NAME "Position"
#define GAME_SHADER_LIGHTS_DIRECTION_UNIFORM_NAME "Direction"
#define GAME_SHADER_LIGHTS_ATTENUATION_UNIFORM_NAME "Attenuation"
#define GAME_SHADER_LIGHTS_SPOTRADIUS_UNIFORM_NAME "SpotRadius"
#define GAME_SHADER_LIGHTCOUNT_UNIFORM_NAME "LightCount"

#define GAME_SHADER_MODELVIEWPROJ_UNIFORM_NAME "ModelViewProj"
#define GAME_SHADER_DIFFTEX_UNIFORM_NAME "DiffTex"
#define GAME_SHADER_MODEL_UNIFORM_NAME "Model"
#define GAME_SHADER_EYEPOS_UNIFORM_NAME "EyePos"
#define GAME_SHADER_DIFFCOL_UNIFORM_NAME "DiffuseColor"
#define GAME_SHADER_SPECCOL_UNIFORM_NAME "SpecularColor"
#define GAME_SHADER_SPECEXP_UNIFORM_NAME "SpecularExp"
#define GAME_SHADER_AMBCOL_UNIFORM_NAME "AmbientColor"
#define GAME_SHADER_FS_PATH SHADER_DIR"/fsmodel.glsl"
#define GAME_SHADER_VS_PATH SHADER_DIR"/vsmodel.glsl"

class GameShader: public Shader
{
	//Static Variables
private:
	static GameShader* pInstance;

	//Member Variables
private:
	Matrix mModel;
	bool mModelChanged;
	GLint mModelLoc;

	Matrix mViewProj;
	bool mViewProjChanged;
	GLint mModelViewProjLoc;

	GLuint mDiffTexId;
	GLint mDifftexLoc;

	Vector mEyePos;
	GLint mEyePosLoc;
	bool mEyePosChanged;

	Vector mDiffCol;
	GLint mDiffColLoc;
	bool mDiffColChanged;

	Vector mSpecCol;
	GLint mSpecColLoc;
	bool mSpecColChanged;

	Vector mAmbCol;
	GLint mAmbColLoc;
	bool mAmbColChanged;

	float mSpecExp = 1;
	GLint mSpecExpLoc = 0;
	bool mSpecExpChanged = true;

	struct LightLoc {
		GLint typeLoc;
		GLint colorLoc;
		GLint positionLoc;
		GLint directionLoc;
		GLint attenuationLoc;
		GLint spotRadiusLoc;
	};

	Light mLights[GAME_SHADER_MAX_LIGHTS];
	struct LightLoc mLightLocs[GAME_SHADER_MAX_LIGHTS];
	unsigned int mLightCount = 0;
	bool mLightsChanged = true;
	GLint mLightCountLoc = 0;


	//Member Funcions
public:
	GameShader(const GameShader& other) = delete;
	virtual ~GameShader() = default;
	static GameShader* GetInstance();
	bool operator=(const GameShader& other) = delete;
	virtual void activate() override;
	void setViewProj(const Matrix& matrix);
	void setModel(const Matrix& matrix);
	void setDiffTex(GLuint texId);
	void setEyePos(const Vector& eyePos);
	void setDiffCol(const Vector& diffCol);
	void setSpecCol(const Vector& specCol);
	void setSpecExp(const float& f);
	void setAmbCol(const Vector& ambCol);
	void addLight(const Light& light);
protected:
	GameShader();
};

