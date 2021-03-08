#pragma once
#include "Collider.h"

class SphereCollider : public Collider
{
	Matrix mRelativeTransform;
public:
	SphereCollider();
	virtual ~SphereCollider() override = default;
	void debugDraw(const Matrix & transform) const override;
	bool collidesWith(const Collider* pOther) const override;
};

