#pragma once

#include "math/Matrix.h"
#include "renderer/Model.h"
#include "Collider.h"

class GameObject
{
private:
	Matrix mTransform;
	Model* pModel;
	Collider* collider;

public:
	void draw(Camera* pCamera);
	// TODO Collider Stuff
};

