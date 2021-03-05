#pragma once
#include "Shader.h"
#include "../Directories.h"

#define CONSTANT_SHADER_COLOR_UNIFORM_NAME "Color"
#define CONSTANT_SHADER_MODELVIEWPROJ_UNIFORM_NAME "ModelViewProj"
#define CONSTANT_SHADER_FS_PATH SHADER_DIR"/fsconstant.glsl"
#define CONSTANT_SHADER_VS_PATH SHADER_DIR"/vsconstant.glsl"

class ConstantShader : public Shader
{
	//Static Variables
private:
	static ConstantShader* pInstance;

	//Member Variables
private:
	float mR;
	float mG;
	float mB;
	bool mColorChanged;
	GLint mColorLoc;

	Matrix mModelViewProj;
	bool mModelViewProjChanged;
	GLint mModelViewProjLoc;

	//Member Funcions
public:
	ConstantShader(const ConstantShader& other) = delete;
	virtual ~ConstantShader();
	static ConstantShader* GetInstance();
	bool operator=(const ConstantShader& other) = delete;
	virtual void activate() override;
	void setColor(float r, float g, float b);
	void setModelViewProj(const Matrix& matrix);
protected:
	ConstantShader();
	
};

