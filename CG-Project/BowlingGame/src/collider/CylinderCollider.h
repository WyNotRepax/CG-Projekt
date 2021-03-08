#pragma once
#include "Collider.h"

class CylinderCollider: public Collider
{
	Matrix mRelativeTransform;
public:
	CylinderCollider();
	virtual ~CylinderCollider() override = default;
	void debugDraw(const Matrix& transform) const override;
	bool collidesWith(const Collider* pOther) const override;
};

