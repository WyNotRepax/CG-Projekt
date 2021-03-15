#include "GameObject.h"
#include "../shader/GameShader.h"

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
	if (mVelocity.magnitudeSquared() < VELOCITY_THRESHOLD*VELOCITY_THRESHOLD) {
		mVelocity = Vector(0, 0, 0);
	}
}

void GameObject::draw(Camera* pCamera, Shader* pShader) {
    if (!mDraw) {
        return;
    }
	if (pModel) {
		pModel->draw(pCamera, Matrix().translation(mPosition), pShader);
	}
}

bool GameObject::collidesWith(GameObject* pOther) {
    if (!mPhysics) {
        return false;
    }
    if (!pOther->mPhysics) {
        return false;
    }
	return (mPosition - pOther->mPosition).magnitude() < mRadius + pOther->mRadius;
}

bool GameObject::collideWith(GameObject* pOther) {
    Vector relativeVelocity = getRelativeVelocity(pOther);
    Vector relativePosition = getRelativePosition(pOther);

    if (relativePosition.dot(relativeVelocity) >= 0) {
        // Objects are moving away (Collision has already been resolved)
        return false;
    }
    relativePosition.normalize();
    float v1 = relativePosition.dot(mVelocity);
    mVelocity -= relativePosition * v1;
    float v2 = relativePosition.dot(pOther->mVelocity);
    pOther->mVelocity -= relativePosition * v2;

    float u_ = ((pOther->mInverseMass * v1) + (mInverseMass * v2)) / (mInverseMass + pOther->mInverseMass);

    float v1_ = 2 * u_ - v1;
    float v2_ = 2 * u_ - v2;

    mVelocity += relativePosition *  v1_;
    pOther->mVelocity += relativePosition * v2_;
	return true;
}

Vector GameObject::getRelativePosition(GameObject* pOther) const {
    return pOther->mPosition - mPosition;
}

Vector GameObject::getRelativeVelocity(GameObject* pOther) const {
    return pOther->mVelocity - mVelocity;
}

void GameObject::setPosition(const Vector& v)
{
    mPosition = v;
}

Vector GameObject::getPosition() const {
    return mPosition;
}

Vector GameObject::getVelocity() const
{
    return mVelocity;
}

void GameObject::activate()
{
    setActive(true);
}

void GameObject::deactivate() {
    setActive(false);
}

bool GameObject::getActive()
{
    return mDraw;
}

void GameObject::setActive(bool active)
{
    mDraw = active;
}
