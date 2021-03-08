#pragma once
#include "Shader.h"
#include "../Directories.h"

#define GAME_SHADER_MODELVIEWPROJ_UNIFORM_NAME "ModelViewProj"
#define GAME_SHADER_FS_PATH SHADER_DIR"/fsmodel.glsl"
#define GAME_SHADER_VS_PATH SHADER_DIR"/vsmodel.glsl"

class GameShader: public Shader
{
	//Static Variables
private:
	static GameShader* pInstance;

	//Member Variables
private:
	Matrix mModelViewProj;
	bool mModelViewProjChanged;
	GLint mModelViewProjLoc;

	//Member Funcions
public:
	GameShader(const GameShader& other) = delete;
	virtual ~GameShader() = default;
	static GameShader* GetInstance();
	bool operator=(const GameShader& other) = delete;
	virtual void activate() override;
	void setModelViewProj(const Matrix& matrix);
protected:
	GameShader();
};

