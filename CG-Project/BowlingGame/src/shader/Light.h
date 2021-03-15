#pragma once
#include "../math/Vector.h"
#include "../math/Matrix.h"


class Light
{
public:
	enum LightType
	{
		POINT = 0,
		DIRECTIONAL = 1,
		SPOT = 2
	};
	LightType Type = LightType::POINT;
	Vector Color = Vector(1,1,1);
	Vector Position = Vector(0,0,0);
	Vector Direction = Vector(1, 0, 0);
	Vector Attenuation = Vector(1, 1, 1);
	Vector SpotRadius = Vector(1, 1, 1);

	Matrix getView() const;
	Matrix getProj() const;
};

