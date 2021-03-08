#pragma once
#include "../renderer/Camera.h"

class Collider
{
protected:
	Collider() = default;
	virtual ~Collider() = default;
public:
	virtual void debugDraw(const Matrix& transform) const = 0;
	virtual bool collidesWith(const Collider* pOther) const = 0;
};

