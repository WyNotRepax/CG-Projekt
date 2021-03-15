#pragma once
#include "../shader/Shader.h"
#include "Camera.h"
#include <assimp/scene.h>

class Model
{
public:
	Matrix mTransform;

private:

	struct Mesh {
		GLuint vaoId;
		GLuint indexBufferId;
		Matrix transform;
		unsigned int indexCount;
		unsigned int materialIndex;
	};
	struct Mesh* mMeshes;
	unsigned int mMeshCount;

	struct Material {
		GLuint texId;
		Vector specularColor;
		Vector diffuseColor;
		Vector ambientColor;
		float specularExp;
	};
	struct Material* mMaterials;
	unsigned int mMaterialCount;

public:
	Model(const std::string& path);
	~Model();
	void draw(const Camera* pCamera, const Matrix& parentTransform, Shader* pShader) const;

private:
	bool load(std::string path);
	bool loadMesh(const aiScene* pScene, unsigned int meshIndex);
	bool loadMaterials(const aiScene* pScene, const std::string& path);
	bool loadMaterial(const aiScene* pScene, unsigned int materialIndex, const std::string& path);
	bool loadMeshes(const aiScene* pScene);
	void loadTransform(const aiScene* pScene);
	void loadTransformRecurse(const aiNode* pNode, const Matrix& parentTransform);
};