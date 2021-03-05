#include "VectorMath.h"

Vector VectorMath::reflection(const Vector& vector, const Vector& normal)
{
	return vector - normal * (2 * normal.dot(vector));
}

float VectorMath::pointLineDistance(const Vector& point, const Vector& lineBase, const Vector& lineDirection, float& x)
{
	Vector lineBaseToPoint = point - lineBase;
	x = lineDirection.dot(lineBaseToPoint);
	Vector closestPointOnLine = lineDirection * x;
	Vector closestPointToPoint = point - closestPointOnLine;
	return closestPointToPoint.magnitude();
}

float VectorMath::lineLineDistance(const Vector& lineBase1, const Vector& lineDirection1, const Vector& lineBase2, const Vector& lineDirection2, float& x1, float& x2)
{
	Vector baseToBase = lineBase1 - lineBase2;
	float a = lineDirection1.dot(lineDirection2);
	if (a == 1.0f) {
		// Lines Are Parallel
		x1 = 0.0f;
		return pointLineDistance(lineBase1, lineBase2, lineDirection2, x2);
	}

	float b = baseToBase.dot(lineDirection1);
	float c = baseToBase.dot(lineDirection2);

	float aSquaredMinus1 = a * a - 1.0f; // != 0 because of the check earlier

	x1 = (a * c - b) / aSquaredMinus1;
	x2 = (c - a * b) / aSquaredMinus1;

	Vector closestPointOnLine1 = lineBase1 + (lineDirection1 * x1);
	Vector closestPointOnLine2 = lineBase2 + (lineDirection2 * x2);

	Vector lineToLine = closestPointOnLine2 - closestPointOnLine1;
	return lineToLine.magnitude();
}
