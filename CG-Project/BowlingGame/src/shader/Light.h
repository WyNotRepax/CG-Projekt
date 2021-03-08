#pragma once
#include "../math/Vector.h"



class Light
{
public:
	enum LightType
	{
		POINT = 0
	};
	LightType Type = LightType::POINT;
	Vector Color = Vector(1,1,1);
	Vector Position = Vector(0,0,0);
	Vector Direction = Vector(1, 0, 0);;
	Vector Attenuation = Vector(1, 1, 1);;
	Vector SpotRadius = Vector(1, 1, 1);;
};

