#include "BowlingBall.h"
#include "../Directories.h"
#include "../renderer/DebugRenderer.h"
#include "../Logging.h"



BowlingBall::BowlingBall() :GameObject() {
	mRadius = 0.2183 / 2;
	mInverseMass = 1 / 3;
	pModel = new Model(MODEL_DIR"/ball.dae");
	mDrag = 0.01;
	mDebugStaticTransform = Matrix().translation(0, mRadius, 0) * Matrix().scale(mRadius);
}

void BowlingBall::draw(Camera* pCamera, Shader*pShader) {
	//LOG("mVelocity:(%f,%f,%f)\n", mVelocity.X, mVelocity.Y, mVelocity.Z);
	//LOG("mPosition:(%f,%f,%f)\n", mPosition.X, mPosition.Y, mPosition.Z);
	GameObject::draw(pCamera, pShader);
	//DebugRenderer::drawUnitSphere(Vector(), Matrix().translation(mPosition) * mDebugStaticTransform);
	//DebugRenderer::drawLine(mPosition, mPosition + (mVelocity));
}

float sign(float f) {
	if (f == 0) {
		return 0;
	}
	if (f > 0) {
		return 1;
	}
	return -1;
}


void BowlingBall::update(float dt)
{
	if (mPosition.Z < -20) {
		mSimulationState = SimulationState::waiting;
	}
	switch (mSimulationState) {
	case SimulationState::normal:
		if (mPosition.X < -0.53) {
			mSimulationState = SimulationState::movingToGutterLeft;
		}
		else if (mPosition.X > 0.53) {
			mSimulationState = SimulationState::movingInGutterRight;
		}
		break;
	case SimulationState::movingToGutterLeft:
		mVelocity.X = -1;
		if (mPosition.X < -0.69) {
			mSimulationState = SimulationState::movingInGutterLeft;
		}
		break;
	case SimulationState::movingToGutterRight:
		mVelocity.X = 1;
		if (mPosition.X > 0.69) {
			mSimulationState = SimulationState::movingInGutterRight;
		}
		break;
	case SimulationState::movingInGutterLeft:
		mPosition.X = -0.69;
		mVelocity.X = 0;
		mPosition.Y = -0.05;
		if (mVelocity.Z > -1) {
			mVelocity.Z = -1;
		}
		break;
	case SimulationState::movingInGutterRight:
		mPosition.X = 0.69;
		mVelocity.X = 0;
		mPosition.Y = -0.05;
		if (mVelocity.Z > -1) {
			mVelocity.Z = -1;
		}
		break;
	case SimulationState::waiting:
		if (mPosition.Z > -20) {
			mSimulationState = SimulationState::normal;
			break;
		}
		mVelocity = Vector(0,0,0);
		break;
	}
	GameObject::update(dt);
}
