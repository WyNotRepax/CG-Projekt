#pragma once
#include "GameObject.h"
class BowlingBall :
    public GameObject
{
public:
    BowlingBall();
    void draw(Camera* pCamera, Shader* pShader) override;
    void update(float dt) override;
private:

    enum class SimulationState {
        normal,
        movingToGutterLeft,
        movingToGutterRight,
        movingInGutterLeft,
        movingInGutterRight,
        waiting
    };

    SimulationState mSimulationState = SimulationState::normal;

    Matrix mDebugStaticTransform;

};

