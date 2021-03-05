#include "Vector.h"
#include <math.h>
#include <sstream>

const Vector Vector::Right = Vector(1, 0, 0);
const Vector Vector::Left = Vector(-1, 0, 0);
const Vector Vector::Up = Vector(0, 1, 0);
const Vector Vector::Down = Vector(0, -1, 0);
const Vector Vector::Front = Vector(0, 0, 1);
const Vector Vector::Back = Vector(0, 0, -1);

Vector::Vector(float x, float y, float z) :X(x), Y(y), Z(z) {}

Vector::Vector() : Vector(0.0f, 0.0f, 0.0f) {}

float Vector::dot(const Vector& v1, const Vector& v2) {
	return v1.X * v2.X + v1.Y * v2.Y + v1.Z * v2.Z;
}

float Vector::dot(const Vector& v)const {
	return Vector::dot(*this, v);
}

Vector Vector::cross(const Vector& v1, const Vector& v2) {
	return Vector(
		v1.Y * v2.Z - v1.Z * v2.Y,
		v1.Z * v2.X - v1.X * v2.Z,
		v1.X * v2.Y - v1.Y * v2.X
	);
}

Vector Vector::cross(const Vector& v)const {
	return Vector::cross(*this, v);
}

Vector Vector::operator+(const Vector& v) const
{
	return Vector(this->X + v.X, this->Y + v.Y, this->Z + v.Z);
}

Vector Vector::operator+=(const Vector& v)
{
	this->X += v.X;
	this->Y += v.Y;
	this->Z += v.Z;
	return *this;
}

Vector Vector::operator-(const Vector& v) const
{
	return Vector(this->X - v.X, this->Y - v.Y, this->Z - v.Z);
}

Vector Vector::operator-=(const Vector& v)
{
	this->X -= v.X;
	this->Y -= v.Y;
	this->Z -= v.Z;
	return *this;
}

Vector Vector::operator*(float f) const
{
	return Vector(this->X * f, this->Y * f, this->Z * f);
}

Vector Vector::operator*=(float f)
{
	this->X *= f;
	this->Y *= f;
	this->Z *= f;
	return *this;
}

Vector Vector::operator/(float f) const
{
	return *this * (1.0f / f);
}

Vector Vector::operator/=(float f)
{
	return *this *= (1.0f / f);
}

Vector::operator std::string() const
{
	std::stringstream str;
	str << "Vector(" << this->X << ", " << this->Y << ", " << this->Z << ")";
	return str.str();
}

Vector Vector::normalize()
{
	*this /= this->magnitude();
	return *this;
}

float Vector::magnitude() const
{
	return sqrt(this->magnitudeSquared());
}

float Vector::magnitudeSquared() const
{
	return this->dot(*this);
}
