#pragma once
#include "../renderer/Camera.h"

class Collider
{
protected:
	Collider() = default;
	virtual ~Collider() = default;
public:
	virtual void debugDraw2(Camera* pCamera) const { this->debugDraw(pCamera, Matrix().identity()); };
	virtual void debugDraw(Camera* pCamera, const Matrix& transform) const = 0;
	virtual bool collidesWith(const Collider& other) const = 0;
};

