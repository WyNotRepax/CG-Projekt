#include "Pin.h"
#include "../renderer/DebugRenderer.h"

Pin::Pin() :GameObject() {
	mRadius = 0.121 / 2;
	mInverseMass = 1 / 1.5;
	pModel = new Model(MODEL_DIR"/pin.dae");
	mDrag = 0.6;
	float height = 0.383;
	mDebugStaticTransform = Matrix().translation(0, height / 2, 0) * Matrix().scale(mRadius, height / 2, mRadius);
	mPosition = Vector(3, 0, 0);
}

void Pin::draw(Camera* pCamera, Shader*pShader) {
	GameObject::draw(pCamera,pShader);
	//DebugRenderer::drawCylinder(Matrix().translation(mPosition) * mDebugStaticTransform);
};