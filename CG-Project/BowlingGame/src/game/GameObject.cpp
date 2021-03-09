#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
	delete pModel;
}

void GameObject::setVelocity(const Vector& velocity)
{
	mVelocity = velocity;
}

void GameObject::update(float dt)
{
	mPosition += mVelocity * dt;
	mVelocity += (mVelocity * -mDrag)*dt;
	if (mVelocity.magnitude() < VELOCITY_THRESHOLD) {
		mVelocity = Vector(0, 0, 0);
	}
}

void GameObject::draw(Camera* pCamera) {
	if (pModel) {
		pModel->draw(pCamera, Matrix().translation(mPosition));
	}
}