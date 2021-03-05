#pragma once
#include "Collider.h"

class SphereCollider : public Collider
{
public:
	SphereCollider();
	virtual ~SphereCollider() override = default;
	virtual void debugDraw() override;
};

