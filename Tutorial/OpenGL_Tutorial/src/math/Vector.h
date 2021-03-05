#pragma once

#include <string>

class Vector
{
public:

	static const Vector Right;
	static const Vector Left;
	static const Vector Up;
	static const Vector Down;
	static const Vector Front;
	static const Vector Back;

	float X;
	float Y;
	float Z;

	Vector(float x, float y, float z);
	Vector();

	static float dot(const Vector& v1, const Vector& v2);
	float dot(const Vector& other) const;

	static Vector cross(const Vector& v1, const Vector& v2);
	Vector cross(const Vector& v)const;

	Vector operator+(const Vector& v)const;
	Vector operator+=(const Vector& v);

	Vector operator-(const Vector& v)const;
	Vector operator-=(const Vector& v);

	Vector operator*(float f)const;
	Vector operator*=(float f);

	Vector operator/(float f)const;
	Vector operator/=(float f);

	operator std::string() const;

	Vector normalize();

	float magnitude()const;
	float magnitudeSquared()const;
};

