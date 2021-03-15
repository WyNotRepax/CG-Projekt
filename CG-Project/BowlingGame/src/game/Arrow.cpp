#include "Arrow.h"
#include "../Directories.h"

Arrow::Arrow()
{
	mRadius = 0;
	mInverseMass = 0;
	pModel = new Model(MODEL_DIR"/Arrow.dae");
	mDrag = 0;
	mPhysics = false;
	mRotation = 0;
}


void Arrow::draw(Camera* pCamera, Shader*pShader)
{
	pModel->mTransform = Matrix().rotationY(mRotation) * Matrix().scale(1,1, mPowerScale);
	GameObject::draw(pCamera,pShader);
}

void Arrow::setRotation(float rotation)
{
	mRotation = rotation;
}

void Arrow::setPowerScale(float powerScale)
{
	mPowerScale = powerScale;
}
