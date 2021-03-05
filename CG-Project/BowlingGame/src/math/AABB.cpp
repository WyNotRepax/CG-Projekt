#include "AABB.h"

#define _MAX(a,b) (a > b) ? a : b
#define _MIN(a,b) (a < b) ? a : b



AABB::AABB() : Min(0.0f, 0.0f, 0.0f), Max(0.0f, 0.0f, 0.0f)
{
}

AABB::AABB(const Vector& v1, const Vector& v2) :
	Min(_MIN(v1.X, v2.X), _MIN(v1.Y, v2.Y), _MIN(v1.Z, v2.Z)),
	Max(_MAX(v1.X, v2.X), _MAX(v1.Y, v2.Y), _MAX(v1.Z, v2.Z))
{

}

AABB& AABB::merge(const AABB& aabb)
{
	Min.X = _MIN(Min.X, aabb.Min.X);
	Min.Y = _MIN(Min.Y, aabb.Min.Y);
	Min.Z = _MIN(Min.Z, aabb.Min.Z);
	Max.X = _MAX(Max.X, aabb.Max.X);
	Max.Y = _MAX(Max.Y, aabb.Max.Y);
	Max.Z = _MAX(Max.Z, aabb.Max.Z);
	return *this;
}

AABB& AABB::operator+=(const AABB& aabb)
{
	return this->merge(aabb);
}

AABB& AABB::operator+(const AABB& aabb) const
{
	return AABB(*this).merge(aabb);
}

AABB AABB::transformed(Matrix m)const
{
	return AABB(m.transformVec4x4(Min), m.transformVec4x4(Max));
}

bool AABB::isInside(const Vector& v)const {
	return
		v.X >= Min.X && v.X <= Max.X &&
		v.Y >= Min.Y && v.Y <= Max.Y &&
		v.Z >= Min.Z && v.Z <= Max.Z;
}

bool AABB::overlaps(const AABB& aabb) const{
	return 
		(Min.X <= aabb.Max.X && Max.X >= aabb.Min.X) &&
		(Min.Y <= aabb.Max.Y && Max.Y >= aabb.Max.Y) &&
		(Min.Z <= aabb.Max.Z && Max.Z >= aabb.Max.Z);
}