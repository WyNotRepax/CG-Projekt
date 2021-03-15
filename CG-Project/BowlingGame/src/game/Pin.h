#pragma once
#include "GameObject.h"
class Pin :
    public GameObject
{
public:
    Pin();
    void draw(Camera* pCamera, Shader* pShader) override;
private:
    Matrix mDebugStaticTransform;
};

