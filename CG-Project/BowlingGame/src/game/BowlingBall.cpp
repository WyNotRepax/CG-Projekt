#include "BowlingBall.h"
#include "../Directories.h"
#include "../renderer/DebugRenderer.h"


BowlingBall::BowlingBall() :GameObject() {
	mRadius = 0.2183 / 2;
	mInverseMass = 1 / 5;
	pModel = new Model(MODEL_DIR"/ball.dae");
	mDrag = 0.01;
	mDebugStaticTransform = Matrix().translation(0, mRadius, 0) * Matrix().scale(mRadius);
}

void BowlingBall::draw(Camera* pCamera) {
	GameObject::draw(pCamera);
	DebugRenderer::drawUnitSphere(Vector(), Matrix().translation(mPosition) * mDebugStaticTransform);
};