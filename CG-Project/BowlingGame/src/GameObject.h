#pragma once

#include "math/Matrix.h"
#include "renderer/Model.h"
#include "collider/Collider.h"

class GameObject
{
private:
	Matrix mTransform;
	Model* pModel;
	bool mDestroyModel;
	Collider* pCollider;

public:
	GameObject();
	~GameObject();
	void setModel(Model* pModel, bool destroy = false);

	void draw(Camera* pCamera);
	// TODO Collider Stuff
};

