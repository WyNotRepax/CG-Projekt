#pragma once
#include "../shader/GameShader.h"
#include "Camera.h"
#include <assimp/scene.h>

class Model
{
public:

private:
	GameShader* pShader;
	Matrix m_transform;

	struct Mesh {
		GLuint vaoId;
		GLuint indexBufferId;
		Matrix transform;
		unsigned int indexCount;
	};

	struct Mesh* mMeshes;
	unsigned int mMeshCount;

public:
	Model(const std::string& path);
	void draw(const Camera* camera)const;

private:
	bool load(const std::string& path);
	bool loadMeshes(const aiScene* pScene);
	void loadTransform(const aiScene* pScene);
	void loadTransformRecurse(const aiNode* pNode, const Matrix& parentTransform);
};