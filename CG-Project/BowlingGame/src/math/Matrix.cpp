#include "Matrix.h"
#include <math.h>
#include <assert.h>
#include <sstream>
#include "../Logging.h"
Matrix::Matrix() : Matrix(
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f
)
{
}

Matrix::Matrix(
	float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23,
	float m30, float m31, float m32, float m33
) :
	m00(m00), m01(m01), m02(m02), m03(m03),
	m10(m10), m11(m11), m12(m12), m13(m13),
	m20(m20), m21(m21), m22(m22), m23(m23),
	m30(m30), m31(m31), m32(m32), m33(m33)
{
}

Matrix Matrix::mult(const Matrix& m1, const Matrix& m2)
{
	return Matrix(
		m1.m00 * m2.m00 + m1.m01 * m2.m10 + m1.m02 * m2.m20 + m1.m03 * m2.m30,
		m1.m00 * m2.m01 + m1.m01 * m2.m11 + m1.m02 * m2.m21 + m1.m03 * m2.m31,
		m1.m00 * m2.m02 + m1.m01 * m2.m12 + m1.m02 * m2.m22 + m1.m03 * m2.m32,
		m1.m00 * m2.m03 + m1.m01 * m2.m13 + m1.m02 * m2.m23 + m1.m03 * m2.m33,

		m1.m10 * m2.m00 + m1.m11 * m2.m10 + m1.m12 * m2.m20 + m1.m13 * m2.m30,
		m1.m10 * m2.m01 + m1.m11 * m2.m11 + m1.m12 * m2.m21 + m1.m13 * m2.m31,
		m1.m10 * m2.m02 + m1.m11 * m2.m12 + m1.m12 * m2.m22 + m1.m13 * m2.m32,
		m1.m10 * m2.m03 + m1.m11 * m2.m13 + m1.m12 * m2.m23 + m1.m13 * m2.m33,

		m1.m20 * m2.m00 + m1.m21 * m2.m10 + m1.m22 * m2.m20 + m1.m23 * m2.m30,
		m1.m20 * m2.m01 + m1.m21 * m2.m11 + m1.m22 * m2.m21 + m1.m23 * m2.m31,
		m1.m20 * m2.m02 + m1.m21 * m2.m12 + m1.m22 * m2.m22 + m1.m23 * m2.m32,
		m1.m20 * m2.m03 + m1.m21 * m2.m13 + m1.m22 * m2.m23 + m1.m23 * m2.m33,

		m1.m30 * m2.m00 + m1.m31 * m2.m10 + m1.m32 * m2.m20 + m1.m33 * m2.m30,
		m1.m30 * m2.m01 + m1.m31 * m2.m11 + m1.m32 * m2.m21 + m1.m33 * m2.m31,
		m1.m30 * m2.m02 + m1.m31 * m2.m12 + m1.m32 * m2.m22 + m1.m33 * m2.m32,
		m1.m30 * m2.m03 + m1.m31 * m2.m13 + m1.m32 * m2.m23 + m1.m33 * m2.m33
	);
}

Matrix Matrix::operator*(const Matrix& m) const
{
	return Matrix::mult(*this, m);
}

Matrix& Matrix::operator*=(const Matrix& m) {
	*this = *this * m;
	return *this;
}

Matrix& Matrix::translation(float x, float y, float z)
{
	m00 = 1; m01 = 0; m02 = 0; m03 = x;
	m10 = 0; m11 = 1; m12 = 0; m13 = y;
	m20 = 0; m21 = 0; m22 = 1; m23 = z;
	m30 = 0; m31 = 0; m32 = 0; m33 = 1;
	return *this;
}

Matrix& Matrix::translation(const Vector& v)
{
	return this->translation(v.X, v.Y, v.Z);
}

Vector Matrix::translation() const
{
	return Vector(m03,m13,m23);
}

Matrix& Matrix::rotationX(float angle)
{
	float c = (float)cos(angle);
	float s = (float)sin(angle);
	m00 = 1; m01 = 0; m02 = 0; m03 = 0;
	m10 = 0; m11 = c; m12 = -s; m13 = 0;
	m20 = 0; m21 = s; m22 = c; m23 = 0;
	m30 = 0; m31 = 0; m32 = 0; m33 = 1;
	return *this;
}

Matrix& Matrix::rotationY(float angle)
{
	float c = (float)cos(angle);
	float s = (float)sin(angle);
	//LOG("cos(%f) = %f, sin(%f) = %f\n", angle, c, angle, s);
	m00 = c; m01 = 0; m02 = -s; m03 = 0;
	m10 = 0; m11 = 1; m12 = 0; m13 = 0;
	m20 = s; m21 = 0; m22 = c; m23 = 0;
	m30 = 0; m31 = 0; m32 = 0; m33 = 1;
	return *this;
}

Matrix& Matrix::rotationZ(float angle)
{
	float c = (float)cos(angle);
	float s = (float)sin(angle);
	m00 = c; m01 = s; m02 = 0; m03 = 0;
	m10 = -s; m11 = c; m12 = 0; m13 = 0;
	m20 = 0; m21 = 0; m22 = 1; m23 = 0;
	m30 = 0; m31 = 0; m32 = 0; m33 = 1;
	return *this;
}

Matrix& Matrix::roationAxis(float angle, const Vector& axis) {
	// Precalculate some values
	const float c = (float)cos(angle);
	const float c_ = 1 - c;
	const float s = (float)sin(angle);

	// For ease of reading
	const float& u1 = axis.X;
	const float& u2 = axis.Y;
	const float& u3 = axis.Z;

	// Set values
	m00 = u1 * u1 * c_ + c;			m01 = u1 * u2 * c_ - u3 * s;	m02 = u1 * u3 * c_ + u2 * s;	m03 = 0;
	m10 = u2 * u1 * c_ + u3 * s;	m11 = u2 * u2 * c_ + c;			m12 = u2 * u3 * c_ - u1 * s;	m13 = 0;
	m20 = u3 * u1 * c_ - u2 * s;	m21 = u3 * u2 * c_ + u1 * s;	m22 = u3 * u3 * c_ + c;			m23 = 0;
	m30 = 0;						m31 = 0;						m32 = 0;						m33 = 1;
	return *this;
}

Matrix& Matrix::scale(float x, float y, float z) {
	m00 = x; m01 = 0; m02 = 0; m03 = 0;
	m10 = 0; m11 = y; m12 = 0; m13 = 0;
	m20 = 0; m21 = 0; m22 = z; m23 = 0;
	m30 = 0; m31 = 0; m32 = 0; m33 = 1;
	return *this;
}

Matrix& Matrix::scale(const Vector& xyz) {
	return this->scale(xyz.X, xyz.Y, xyz.Z);
}

Matrix& Matrix::scale(float f) {
	this->scale(f, f, f);
	return *this;
}

Matrix& Matrix::identity() {
	m00 = 1; m01 = 0; m02 = 0; m03 = 0;
	m10 = 0; m11 = 1; m12 = 0; m13 = 0;
	m20 = 0; m21 = 0; m22 = 1; m23 = 0;
	m30 = 0; m31 = 0; m32 = 0; m33 = 1;
	return *this;
}

Matrix& Matrix::invert() {
	const float num5 = m00;
	const float num4 = m01;
	const float num3 = m02;
	const float num2 = m03;
	const float num9 = m10;
	const float num8 = m11;
	const float num7 = m12;
	const float num6 = m13;
	const float num17 = m20;
	const float num16 = m21;
	const float num15 = m22;
	const float num14 = m23;
	const float num13 = m30;
	const float num12 = m31;
	const float num11 = m32;
	const float num10 = m33;
	const float num23 = (num15 * num10) - (num14 * num11);
	const float num22 = (num16 * num10) - (num14 * num12);
	const float num21 = (num16 * num11) - (num15 * num12);
	const float num20 = (num17 * num10) - (num14 * num13);
	const float num19 = (num17 * num11) - (num15 * num13);
	const float num18 = (num17 * num12) - (num16 * num13);
	const float num39 = ((num8 * num23) - (num7 * num22)) + (num6 * num21);
	const float num38 = -(((num9 * num23) - (num7 * num20)) + (num6 * num19));
	const float num37 = ((num9 * num22) - (num8 * num20)) + (num6 * num18);
	const float num36 = -(((num9 * num21) - (num8 * num19)) + (num7 * num18));
	const float num = (float)1 / ((((num5 * num39) + (num4 * num38)) + (num3 * num37)) + (num2 * num36));
	m00 = num39 * num;
	m10 = num38 * num;
	m20 = num37 * num;
	m30 = num36 * num;
	m01 = -(((num4 * num23) - (num3 * num22)) + (num2 * num21)) * num;
	m11 = (((num5 * num23) - (num3 * num20)) + (num2 * num19)) * num;
	m21 = -(((num5 * num22) - (num4 * num20)) + (num2 * num18)) * num;
	m31 = (((num5 * num21) - (num4 * num19)) + (num3 * num18)) * num;
	const float num35 = (num7 * num10) - (num6 * num11);
	const float num34 = (num8 * num10) - (num6 * num12);
	const float num33 = (num8 * num11) - (num7 * num12);
	const float num32 = (num9 * num10) - (num6 * num13);
	const float num31 = (num9 * num11) - (num7 * num13);
	const float num30 = (num9 * num12) - (num8 * num13);
	m02 = (((num4 * num35) - (num3 * num34)) + (num2 * num33)) * num;
	m12 = -(((num5 * num35) - (num3 * num32)) + (num2 * num31)) * num;
	m22 = (((num5 * num34) - (num4 * num32)) + (num2 * num30)) * num;
	m32 = -(((num5 * num33) - (num4 * num31)) + (num3 * num30)) * num;
	const float num29 = (num7 * num14) - (num6 * num15);
	const float num28 = (num8 * num14) - (num6 * num16);
	const float num27 = (num8 * num15) - (num7 * num16);
	const float num26 = (num9 * num14) - (num6 * num17);
	const float num25 = (num9 * num15) - (num7 * num17);
	const float num24 = (num9 * num16) - (num8 * num17);
	m03 = -(((num4 * num29) - (num3 * num28)) + (num2 * num27)) * num;
	m13 = (((num5 * num29) - (num3 * num26)) + (num2 * num25)) * num;
	m23 = -(((num5 * num28) - (num4 * num26)) + (num2 * num24)) * num;
	m33 = (((num5 * num27) - (num4 * num25)) + (num3 * num24)) * num;
	return *this;
}

float Matrix::determinant() {
	return m00 * (m11 * m22 - m12 * m21) -
		m01 * (m10 * m22 - m12 * m20) +
		m02 * (m10 * m21 - m11 * m20);
}

Matrix& Matrix::lookAt(const Vector& target, const Vector& up, const Vector& position)
{
	Vector f = target - position;
	f.normalize();
	Vector u = up;
	u.normalize();
	Vector r = f.cross(u);
	r.normalize();
	u = r.cross(f);
	m00 = r.X;
	m01 = r.Y;
	m02 = r.Z;
	m03 = -(r.dot(position));
	m10 = u.X;
	m11 = u.Y;
	m12 = u.Z;
	m13 = -(u.dot(position));
	m20 = -f.X;
	m21 = -f.Y;
	m22 = -f.Z;
	m23 = (f.dot(position));
	m30 = 0;
	m31 = 0;
	m32 = 0;
	m33 = 1;
	return *this;
}

Matrix& Matrix::perspective(float fovY, float aspectRatio, float near, float far)
{
	assert(near < far);

	const float f = 1.0f / (float)tan(fovY * 0.5);
	const float nearMinusFar = near - far;

	m01 = m02 = m03 = 0;
	m10 = m12 = m13 = 0;
	m20 = m21 = 0;
	m30 = m31 = m33 = 0;
	m32 = -1;

	m00 = f / aspectRatio;
	m11 = f;
	m22 = (far + near) / nearMinusFar;
	m23 = 2.0f * far * near / nearMinusFar;
	return *this;
}

Matrix& Matrix::orthographic(float width, float height, float near, float far) {
	float fmn = 1.0f / (far - near);
	m00 = 2.0f / width;
	m01 = 0.0f;
	m02 = 0.0f;
	m03 = 0.0f;
	m10 = 0.0f;
	m11 = 2.0f / height;
	m12 = 0.0f;
	m13 = 0.0f;
	m20 = 0.0f;
	m21 = 0.0f;
	m22 = -2.0f * fmn;
	m23 = -(far + near) * fmn;
	m30 = 0.0f;
	m31 = 0.0f;
	m32 = 0.0f;
	m33 = 1.0f;
	return *this;
}

Vector Matrix::transformVec4x4(const Vector& v) const
{
	float X = m00 * v.X + m01 * v.Y + m02 * v.Z + m03;
	float Y = m10 * v.X + m11 * v.Y + m12 * v.Z + m13;
	float Z = m20 * v.X + m21 * v.Y + m22 * v.Z + m23;
	float W = m30 * v.X + m31 * v.Y + m32 * v.Z + m33;
	return Vector(X / W, Y / W, Z / W);
}

Vector Matrix::transformVec3x3(const Vector& v) const {
	float X = m00 * v.X + m01 * v.Y + m02 * v.Z;
	float Y = m10 * v.X + m11 * v.Y + m12 * v.Z;
	float Z = m20 * v.X + m21 * v.Y + m22 * v.Z;
	return Vector(X, Y, Z);
}

Matrix::operator std::string() const {
	std::stringstream str;
	str << m00 << " " << m01 << " " << m02 << " " << m03 << std::endl;
	str << m10 << " " << m11 << " " << m12 << " " << m13 << std::endl;
	str << m20 << " " << m21 << " " << m22 << " " << m23 << std::endl;
	str << m30 << " " << m31 << " " << m32 << " " << m33 << std::endl;
	return str.str();
}