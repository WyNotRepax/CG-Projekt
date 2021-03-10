#pragma once

#include "../renderer/Model.h"
#include "../math/Vector.h"

#define VELOCITY_THRESHOLD 0.05f

class GameObject
{
protected:

	Model* pModel = nullptr;
	Vector mPosition = Vector(0,0,0);
	Vector mVelocity = Vector(0,0,0);
	float mRadius = 1;
	float mInverseMass = 1;
	float mDrag = 0;

	GameObject();
	virtual ~GameObject();

public:
	void setVelocity(const Vector& velocity);
	void update(float dt);
	virtual void draw(Camera* pCamera);
	bool collidesWith(GameObject* pOther);
	bool collideWith(GameObject* pOther);
	Vector getRelativePosition(GameObject* pOther) const;
	Vector getRelativeVelocity(GameObject* pOther) const;
	void setPosition(const Vector& v);
	Vector getPosition() const;
};

