#pragma once

#include "Vector.h"

class VectorMath
{
	static Vector reflection(const Vector& vector, const Vector& normal);

	// The line is defined as lineBase + x * lineDirection
	// X is set to the value for the closest Point on the Line
	// lineDirection needs to be normalized
	static float pointLineDistance(const Vector& point, const Vector& lineBase, const Vector& lineDirection, float& x);

	// The lines are defined as lineBase + x * lineDirection
	// X is set to the value for the closest Point on the Line
	// lineDirections need to be normalized
	static float lineLineDistance(const Vector& lineBase1, const Vector& lineDirection1, const Vector& lineBase2, const Vector& lineDirection2, float& x1, float& x2);
};

