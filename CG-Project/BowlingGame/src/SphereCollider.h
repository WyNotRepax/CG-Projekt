#pragma once
#include "Collider.h"

class SphereCollider : public Collider
{
	Matrix mRelativeTransform;
	float mRadius;
public:
	SphereCollider();
	virtual ~SphereCollider() override = default;
	void debugDraw(Camera * pCamera, const Matrix & transform) const override;
	bool collidesWith(const Collider& pOther) const override;
};

