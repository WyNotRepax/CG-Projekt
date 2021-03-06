#include "GameObject.h"

GameObject::GameObject() :mTransform(Matrix().identity()), pModel(nullptr), mDestroyModel(false), pCollider(nullptr)
{
}

GameObject::~GameObject()
{
	if (mDestroyModel) {
		delete pModel;
	}
}

void GameObject::setModel(Model* pModel, bool destroy)
{
	if (mDestroyModel && this->pModel != nullptr) {
		delete this->pModel;
	}
	this->mDestroyModel = destroy;
	this->pModel = pModel;
}
