#pragma once
#include "GameObject.h"

class Arrow :
    public GameObject
{
private:
	float mRotation;
	float mPowerScale = 1;
public:
	Arrow();
	void draw(Camera* pCamera, Shader* pShader) override;
	void setRotation(float rotation);
	void setPowerScale(float powerScale);
};

