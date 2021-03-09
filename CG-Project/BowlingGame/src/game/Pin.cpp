#include "Pin.h"
#include "../renderer/DebugRenderer.h"

Pin::Pin() :GameObject() {
	mRadius = 0.5;
	mMass = 1;
	pModel = new Model(MODEL_DIR"/pin.dae");
	mDrag = 0.6;
}

void Pin::draw(Camera* pCamera) {
	GameObject::draw(pCamera);
	DebugRenderer::drawCylinder(Matrix().translation(mPosition) * Matrix().scale(mRadius, 1, mRadius));
};