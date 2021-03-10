#pragma once
#include "GameObject.h"
class BowlingBall :
    public GameObject
{
public:
    BowlingBall();
    void draw(Camera* pCamera) override;
private:
    Matrix mDebugStaticTransform;

};

