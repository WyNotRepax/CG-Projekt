#include "Model.h"
#include <fstream>
#include "../shader/ConstantShader.h"
#include "../Logging.h"
#include <assimp/Importer.hpp>
#include <assimp/mesh.h>

Model::Model(const std::string& path) {
	pShader = ConstantShader::GetInstance();
}

void Model::draw(const Camera* pCamera)const {
	//TODO use correct shader;
	ConstantShader* pShader = dynamic_cast<ConstantShader*>(this->pShader);  // <- REMOVE THIS 
	assert(pShader != nullptr);
	for (unsigned int meshIndex = 0; meshIndex < mMeshCount; meshIndex++) {
		Mesh& currMesh = mMeshes[meshIndex];
		pShader->setModelViewProj(currMesh.transform * pCamera->getViewProj());
		pShader->activate();
		LOG_CALL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, currMesh.indexBufferId);
		LOG_CALL(glBindVertexArray, currMesh.vaoId);
		LOG_CALL(glEnableVertexAttribArray, 0);
		LOG_CALL(glDrawElements, GL_TRIANGLES, currMesh.indexCount, GL_UNSIGNED_INT, 0);
		LOG_CALL(glDisableVertexAttribArray, 0);
	}
	LOG_CALL(glBindVertexArray, 0);
	LOG_CALL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, 0);
}

bool Model::load(const std::string& path)
{
	Assimp::Importer aiImporter;
	const aiScene* pScene = aiImporter.ReadFile(path.c_str(), 0);
	if (pScene == nullptr) {
		LOG("Assimp could not load file %s\n", path.c_str());
		return false;
	}
	if (!loadMeshes(pScene)) {
		LOG("Could not load Meshes from Scene %s\n", path.c_str());
	}
	loadTransform(pScene);
	return true;
}

bool Model::loadMeshes(const aiScene* pScene)
{
	bool err = false;
	mMeshCount = pScene->mNumMeshes;
	mMeshes = new struct Mesh[mMeshCount];
	for (unsigned int meshIndex = 0; meshIndex < mMeshCount; meshIndex++) {
		aiMesh* pCurrMesh = pScene->mMeshes[meshIndex];

		// Load Vertex Data
		float* vertexData = new float[3 * pCurrMesh->mNumVertices];
		float* vertexNormalData = new float[3 * pCurrMesh->mNumVertices];
		for (unsigned int vertexIndex = 0; vertexIndex < pCurrMesh->mNumVertices; vertexIndex++) {
			vertexData[3 * vertexIndex + 0] = pCurrMesh->mVertices[vertexIndex].x;
			vertexData[3 * vertexIndex + 1] = pCurrMesh->mVertices[vertexIndex].y;
			vertexData[3 * vertexIndex + 2] = pCurrMesh->mVertices[vertexIndex].z;
		}
		LOG_CALL(glGenVertexArrays, 1, &mMeshes[meshIndex].vaoId);
		LOG_CALL(glBindVertexArray, mMeshes[meshIndex].vaoId);
		GLuint vertexBufferId;

		LOG_CALL(glGenBuffers, 1, &vertexBufferId);
		LOG_CALL(glBindBuffer, GL_ARRAY_BUFFER, vertexBufferId);
		LOG_CALL(glBufferData, GL_ARRAY_BUFFER, sizeof(float) * 3 * pCurrMesh->mNumVertices, vertexData, GL_STATIC_DRAW);

		LOG_CALL(glVertexAttribPointer, 0, 3, GL_FLOAT, false, 0, 0);

		LOG_CALL(glBindVertexArray, 0);
		// There might be a driver bug that is triggered by that line so don't i guess? 
		// See https://stackoverflow.com/questions/27937285/when-should-i-call-gldeletebuffers
		// LOG_CALL(glDeleteBuffers, 1, &vertexBufferId);
		LOG_CALL(glBindBuffer, GL_ARRAY_BUFFER, 0);

		delete[] vertexData;
		delete[] vertexNormalData;

		// Load Index Data

		mMeshes[meshIndex].indexCount = pCurrMesh->mNumFaces * 3;
		unsigned int* indexData = new unsigned int[pCurrMesh->mNumFaces * 3];
		for (unsigned int faceIndex = 0; faceIndex < pCurrMesh->mNumFaces; faceIndex++) {
			if (pCurrMesh->mFaces[faceIndex].mNumIndices != 3) {
				LOG("Skipped Mesh %d, Face %d because it has %d indices (expected 3)\n", meshIndex, faceIndex, pCurrMesh->mFaces[faceIndex].mNumIndices);
				err = true;
				indexData[faceIndex * 3 + 0] = 0;
				indexData[faceIndex * 3 + 1] = 0;
				indexData[faceIndex * 3 + 2] = 0;
			}
			else {
				for (unsigned int i = 0; i < 3; i++) {
					indexData[faceIndex * 3 + i] = pCurrMesh->mFaces[faceIndex].mIndices[i];
				}
			}
		}

		LOG_CALL(glGenBuffers, 1, &mMeshes[meshIndex].indexBufferId);
		LOG_CALL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, mMeshes[meshIndex].indexBufferId);
		LOG_CALL(glBufferData, GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mMeshes[meshIndex].indexCount, indexData, GL_STATIC_DRAW);
		LOG_CALL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, 0);
		delete[] indexData;
	}
	return err;
}

void Model::loadTransform(const aiScene* pScene)
{
	loadTransformRecurse(pScene->mRootNode, Matrix().identity());
}

Matrix ConvertMatrix(const aiMatrix4x4& matrix) {
	return Matrix(
		matrix.a1, matrix.a2, matrix.a3, matrix.a4,
		matrix.b1, matrix.b2, matrix.b3, matrix.b4,
		matrix.c1, matrix.c2, matrix.c3, matrix.c4,
		matrix.d1, matrix.d2, matrix.d3, matrix.d4
	);
}
void Model::loadTransformRecurse(const aiNode* pNode, const Matrix& parentTransform)
{
	Matrix currentTransform = parentTransform * ConvertMatrix(pNode->mTransformation);
	for (unsigned int meshIndex = 0; meshIndex < pNode->mNumMeshes; meshIndex++) {
		mMeshes[pNode->mMeshes[meshIndex]].transform = currentTransform;
	}
	for (unsigned int childIndex = 0; childIndex < pNode->mNumChildren; childIndex++) {
		loadTransformRecurse(pNode->mChildren[childIndex], currentTransform);
	}
}
