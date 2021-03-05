#pragma once
class Collider
{
protected:
	Collider() = default;
	virtual ~Collider() = default;
public:
	virtual void debugDraw() = 0;
	virtual void collidesWith(const Collider& other) = 0;
};

