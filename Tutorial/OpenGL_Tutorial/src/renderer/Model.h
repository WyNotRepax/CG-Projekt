#pragma once
#include "../shader/Shader.h"
#include "Camera.h"

class Model
{
public:

private:
	Shader* pShader;
	Matrix m_transform;

public:
	void draw(const Camera* camera)const;

private:
	Model();
};

