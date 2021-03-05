#pragma once

#include "Vector.h"
#include "Matrix.h"

class AABB
{
public:
	Vector Max;
	Vector Min;

	AABB();
	AABB(const Vector& v1, const Vector& v2);

	AABB& merge(const AABB& aabb);
	AABB& operator+=(const AABB& aabb);
	AABB& operator+(const AABB& aabb) const;

	AABB transformed(Matrix m)const;
	bool isInside(const Vector& v) const;
	bool overlaps(const AABB& aabb) const;
};

