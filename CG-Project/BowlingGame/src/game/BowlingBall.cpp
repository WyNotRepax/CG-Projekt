#include "BowlingBall.h"
#include "../Directories.h"
#include "../renderer/DebugRenderer.h"


BowlingBall::BowlingBall() :GameObject() {
	mRadius = 1;
	mMass = 1;
	pModel = new Model(MODEL_DIR"/ball.dae");
	mDrag = 0.6;
}

void BowlingBall::draw(Camera* pCamera) {
	GameObject::draw(pCamera);
	DebugRenderer::drawSphere(Vector(0, 0, 0), mRadius, Matrix().translation(mPosition));
};