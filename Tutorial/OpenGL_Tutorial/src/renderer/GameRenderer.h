#pragma once
#include <vector>
#include "Model.h"
#include "Camera.h"

class GameRenderer
{
public:
	void render()const;
	void addModel(Model* model);
	GameRenderer(Camera* pCamera);
private:
	std::vector<Model*> mModelList;
	Camera* pCamera;
};

