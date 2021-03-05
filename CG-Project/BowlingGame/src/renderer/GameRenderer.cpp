#include "GameRenderer.h"

void GameRenderer::render() const
{
}

void GameRenderer::addModel(Model* model)
{
	mModelList.push_back(model);
}

GameRenderer::GameRenderer(Camera* pCamera):pCamera(pCamera)
{
	for (auto pModel : mModelList) {
		pModel->draw(pCamera);
	}
}
