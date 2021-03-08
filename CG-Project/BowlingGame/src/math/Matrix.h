#pragma once
#include "Vector.h"
#include <string>
class Matrix
{
public:
	// This holds the Matrix values
	// Accesible as individual variables mxx or as an array

	union {
		struct {
			float m00, m10, m20, m30;
			float m01, m11, m21, m31;
			float m02, m12, m22, m32;
			float m03, m13, m23, m33;
		};
		struct
		{
			float m[16];
		};
	};
	Matrix();
	Matrix(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33
	);

	static Matrix mult(const Matrix& m1, const Matrix& m2);
	Matrix operator* (const Matrix& m)const;
	Matrix& operator*= (const Matrix& m);

	Matrix& translation(float x, float y, float z);
	Matrix& translation(const Vector& v);
	Vector translation()const;

	Matrix& rotationX(float angle);
	Matrix& rotationY(float angle);
	Matrix& rotationZ(float angle);

	Matrix& roationAxis(float angle, const Vector& axis);

	Matrix& scale(float x, float y, float z);
	Matrix& scale(const Vector& xyz);
	Matrix& scale(float s);

	Matrix& invert();
	float determinant();

	Matrix& identity();

	Matrix& lookAt(const Vector& target, const Vector& up, const Vector& position);
	Matrix& perspective(float fovY, float aspectRatio, float nearPlane, float farPlane);
	Matrix& orthographic(float width, float height, float near, float far);

	Vector transformVec4x4(const Vector& v) const;
	Vector transformVec3x3(const Vector& v) const;

	operator std::string() const;
};

