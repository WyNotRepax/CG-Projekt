#pragma once
#include "Shader.h"
#include "../Directories.h"

#define GAME_SHADER_MODELVIEWPROJ_UNIFORM_NAME "ModelViewProj"
#define GAME_SHADER_DIFFTEX_UNIFORM_NAME "DiffTex"
#define GAME_SHADER_MODEL_UNIFORM_NAME "Model"
#define GAME_SHADER_EYEPOS_UNIFORM_NAME "EyePos"
#define GAME_SHADER_DIFFCOL_UNIFORM_NAME "DiffuseColor"
#define GAME_SHADER_SPECCOL_UNIFORM_NAME "SpecularColor"
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
	void setAmbCol(const Vector& ambCol);
protected:
	GameShader();
};

