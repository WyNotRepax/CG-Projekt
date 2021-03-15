#include "Model.h"
#include <fstream>
#include "../shader/ConstantShader.h"
#include "../Logging.h"
#include <assimp/Importer.hpp>
#include <assimp/mesh.h>
#include <assimp/material.h>
#include <FreeImage.h>
#include "../shader/ShadowShader.h"
#include "../shader/GameShader.h"

Model::Model(const std::string& path) :mMaterials(nullptr), mMeshes(nullptr), mTransform(Matrix().identity()) {
	bool loaded = load(path);
	assert(loaded);
}

Model::~Model()
{
	delete[] mMeshes;
	mMeshCount = 0;
	delete[] mMaterials;
	mMaterialCount = 0;
}

void Model::draw(const Camera* pCamera, const Matrix& parentTransform, Shader* pShader)const {
	GameShader* pGameShader = dynamic_cast<GameShader*>(pShader);
	ShadowShader* pShadowShader = dynamic_cast<ShadowShader*>(pShader);

	for (unsigned int meshIndex = 0; meshIndex < mMeshCount; meshIndex++) {
		Mesh& currMesh = mMeshes[meshIndex];



		//LOG("TRANSFORM:\n %s\n", ((std::string)currMesh.transform).c_str());
		if (pGameShader) {
			pGameShader->setDiffTex(mMaterials[currMesh.materialIndex].texId);
			pGameShader->setAmbCol(mMaterials[currMesh.materialIndex].ambientColor);
			pGameShader->setDiffCol(mMaterials[currMesh.materialIndex].diffuseColor);
			pGameShader->setSpecCol(mMaterials[currMesh.materialIndex].specularColor);
			pGameShader->setViewProj(pCamera->getViewProj());
		}
		else if (pShadowShader) {
			pShadowShader->setModelViewProj(pCamera->getViewProj() * (parentTransform * mTransform * currMesh.transform));
		}
		Matrix invView = pCamera->getView();
		invView.invert();
		if (pGameShader) {
			pGameShader->setEyePos(invView.translation());
			pGameShader->setModel(parentTransform * mTransform * currMesh.transform);
		}
		pShader->activate();


		LOG_CALL(glBindVertexArray, currMesh.vaoId);
		LOG_CALL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, currMesh.indexBufferId);
		LOG_CALL(glEnableVertexAttribArray, 0);
		LOG_CALL(glEnableVertexAttribArray, 1);
		LOG_CALL(glEnableVertexAttribArray, 2);
		LOG_CALL(glDrawElements, GL_TRIANGLES, currMesh.indexCount, GL_UNSIGNED_INT, 0);
		LOG_CALL(glDisableVertexAttribArray, 0);
		LOG_CALL(glDisableVertexAttribArray, 1);
		LOG_CALL(glDisableVertexAttribArray, 2);
		LOG_CALL(glBindVertexArray, 0);

	}
	LOG_CALL(glBindVertexArray, 0);
	LOG_CALL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, 0);
}



bool Model::load(std::string path)
{
	//LOG("Loading %s\n\n", path.c_str());
	Assimp::Importer aiImporter;
	const aiScene* pScene = aiImporter.ReadFile(path.c_str(), 0);
	if (pScene == nullptr) {
		LOG("Assimp could not load file %s\n", path.c_str());
		return false;
	}
	if (!loadMeshes(pScene)) {
		LOG("Could not load Meshes from Scene %s\n", path.c_str());
	}
	size_t pos = path.rfind('/');
	if (pos == std::string::npos)
		pos = path.rfind('\\');
	if (pos != std::string::npos)
		path.resize(pos + 1);
	loadTransform(pScene);
	loadMaterials(pScene, path);
	return true;
}

bool Model::loadMesh(const aiScene* pScene, unsigned int meshIndex) {

	bool success = true;
	aiMesh* pAiMesh = pScene->mMeshes[meshIndex];
	Mesh* pMesh = &mMeshes[meshIndex];
	pMesh->materialIndex = pAiMesh->mMaterialIndex;
	//LOG("Loading Mesh %s %d vertices. Material Index:%d\n", pAiMesh->mName.C_Str(), pAiMesh->mNumVertices,pAiMesh->mMaterialIndex);
	// Load Vertex Data
	float* vertexData = new float[3 * pAiMesh->mNumVertices];
	float* vertexNormalData = new float[3 * pAiMesh->mNumVertices];
	float* vertexTextureData = new float[2 * pAiMesh->mNumVertices];
	for (unsigned int vertexIndex = 0; vertexIndex < pAiMesh->mNumVertices; vertexIndex++) {
		// Load Vertex Position Data
		vertexData[3 * vertexIndex + 0] = pAiMesh->mVertices[vertexIndex].x;
		vertexData[3 * vertexIndex + 1] = pAiMesh->mVertices[vertexIndex].y;
		vertexData[3 * vertexIndex + 2] = pAiMesh->mVertices[vertexIndex].z;
		// Load Vertex Normal Data
		vertexNormalData[3 * vertexIndex + 0] = pAiMesh->mNormals[vertexIndex].x;
		vertexNormalData[3 * vertexIndex + 1] = pAiMesh->mNormals[vertexIndex].y;
		vertexNormalData[3 * vertexIndex + 2] = pAiMesh->mNormals[vertexIndex].z;
		// Load Texture Coordinates
		vertexTextureData[2 * vertexIndex + 0] = pAiMesh->mTextureCoords[0][vertexIndex].x;
		vertexTextureData[2 * vertexIndex + 1] = pAiMesh->mTextureCoords[0][vertexIndex].y;
		//LOG("NORMAL %d: %f,%f,%f\n", vertexIndex, pCurrAIMesh->mNormals[vertexIndex].x, pCurrAIMesh->mNormals[vertexIndex].y, pCurrAIMesh->mNormals[vertexIndex].z);
	}

	LOG_CALL(glGenVertexArrays, 1, &pMesh->vaoId);
	LOG_CALL(glBindVertexArray, pMesh->vaoId);

	GLuint vertexBufferId;
	LOG_CALL(glGenBuffers, 1, &vertexBufferId);
	LOG_CALL(glBindBuffer, GL_ARRAY_BUFFER, vertexBufferId);
	LOG_CALL(glBufferData, GL_ARRAY_BUFFER, sizeof(float) * 3 * pAiMesh->mNumVertices, vertexData, GL_STATIC_DRAW);
	LOG_CALL(glVertexAttribPointer, 0, 3, GL_FLOAT, false, sizeof(float) * 3, 0);

	GLuint vertexNormalBufferId;
	LOG_CALL(glGenBuffers, 1, &vertexNormalBufferId);
	LOG_CALL(glBindBuffer, GL_ARRAY_BUFFER, vertexNormalBufferId);
	LOG_CALL(glBufferData, GL_ARRAY_BUFFER, sizeof(float) * 3 * pAiMesh->mNumVertices, vertexNormalData, GL_STATIC_DRAW);
	LOG_CALL(glVertexAttribPointer, 1, 3, GL_FLOAT, false, sizeof(float) * 3, 0);

	GLuint vertexTextureCoordId;
	LOG_CALL(glGenBuffers, 1, &vertexTextureCoordId);
	LOG_CALL(glBindBuffer, GL_ARRAY_BUFFER, vertexTextureCoordId);
	LOG_CALL(glBufferData, GL_ARRAY_BUFFER, sizeof(float) * 2 * pAiMesh->mNumVertices, vertexTextureData, GL_STATIC_DRAW);
	LOG_CALL(glVertexAttribPointer, 2, 2, GL_FLOAT, false, sizeof(float) * 2, 0);

	LOG_CALL(glBindVertexArray, 0);
	// There might be a driver bug that is triggered by that line so don't i guess? 
	// See https://stackoverflow.com/questions/27937285/when-should-i-call-gldeletebuffers
	// LOG_CALL(glDeleteBuffers, 1, &vertexBufferId);
	LOG_CALL(glBindBuffer, GL_ARRAY_BUFFER, 0);

	delete[] vertexData;
	delete[] vertexNormalData;
	delete[] vertexTextureData;

	// Load Index Data

	mMeshes[meshIndex].indexCount = pAiMesh->mNumFaces * 3;
	unsigned int* indexData = new unsigned int[pAiMesh->mNumFaces * 3];
	for (unsigned int faceIndex = 0; faceIndex < pAiMesh->mNumFaces; faceIndex++) {
		aiFace* pFace = &pAiMesh->mFaces[faceIndex];
		if (pFace->mNumIndices != 3) {
			LOG("Skipped Mesh %d, Face %d because it has %d indices (expected 3)\n", meshIndex, faceIndex, pFace->mNumIndices);
			success = false;
			indexData[faceIndex * 3 + 0] = 0;
			indexData[faceIndex * 3 + 1] = 0;
			indexData[faceIndex * 3 + 2] = 0;
		}
		else {
			for (unsigned int i = 0; i < 3; i++) {
				indexData[faceIndex * 3 + i] = pFace->mIndices[i];
			}
		}
	}
	LOG_CALL(glGenBuffers, 1, &pMesh->indexBufferId);
	LOG_CALL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, pMesh->indexBufferId);
	LOG_CALL(glBufferData, GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * pMesh->indexCount, indexData, GL_STATIC_DRAW);
	LOG_CALL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, 0);
	delete[] indexData;
	return success;
}

bool Model::loadMaterials(const aiScene* pScene, const std::string& path) {
	mMaterialCount = pScene->mNumMaterials;
	mMaterials = new struct Material[mMaterialCount];
	//LOG("Loading %d materials\n", mMaterialCount);
	for (unsigned int materialIndex = 0; materialIndex < mMaterialCount; materialIndex++) {
		loadMaterial(pScene, materialIndex, path);
	}
	return true; // TODO
}

bool Model::loadMaterial(const aiScene* pScene, unsigned int materialIndex, const std::string& path) {
	//LOG("Loading Material %d\n", materialIndex);
	aiMaterial* pAiMaterial = pScene->mMaterials[materialIndex];
	struct Material* pMaterial = mMaterials + materialIndex;
	aiColor3D c;
	pAiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, c);
	pMaterial->diffuseColor = Vector(c.r, c.g, c.b);
	pAiMaterial->Get(AI_MATKEY_COLOR_SPECULAR, c);
	pMaterial->specularColor = Vector(c.r, c.g, c.b);
	pAiMaterial->Get(AI_MATKEY_COLOR_AMBIENT, c);
	pMaterial->ambientColor = Vector(c.r, c.g, c.b);
	pAiMaterial->Get(AI_MATKEY_SHININESS, pMaterial->specularExp);
	//LOG("diffuseColor=(%f,%f,%f),specularColor=(%f,%f,%f),ambientColor=(%f,%f,%f)\n",
	//	pMaterial->diffuseColor.X, pMaterial->diffuseColor.Y, pMaterial->diffuseColor.Z,
	//	pMaterial->specularColor.X, pMaterial->specularColor.Y, pMaterial->specularColor.Z,
	//	pMaterial->ambientColor.X, pMaterial->ambientColor.Y, pMaterial->ambientColor.Z);

	//Load Texture
	aiString fileName;
	pAiMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &fileName);
	unsigned int width;
	unsigned int height;
	unsigned char* data;
	if (fileName.length > 0) {

		std::string fullPath = path + std::string(fileName.C_Str());
		FREE_IMAGE_FORMAT imageFormat = FreeImage_GetFileType(fullPath.c_str());
		FIBITMAP* pBitMap = FreeImage_Load(imageFormat, fullPath.c_str());
		if (pBitMap == nullptr) {
			//LOG("Could not open %s\n", fullPath.c_str());
			pMaterial->texId = 0;
			return false;
		}
		width = FreeImage_GetWidth(pBitMap);
		height = FreeImage_GetHeight(pBitMap);
		unsigned int bpp = FreeImage_GetBPP(pBitMap);

		data = new unsigned char[width * height * 4];
		for (unsigned int y = 0; y < height; y++) {
			for (unsigned int x = 0; x < width; x++) {
				RGBQUAD c;
				FreeImage_GetPixelColor(pBitMap, x, y, &c);
				unsigned int index = 4 * (y * width + x);
				data[index + 0] = c.rgbRed;
				data[index + 1] = c.rgbGreen;
				data[index + 2] = c.rgbBlue;
				data[index + 3] = (bpp == 32) ? c.rgbReserved : 255;
			}
		}
		FreeImage_Unload(pBitMap);
	}
	else {
		width = 1;
		height = 1;
		data = new unsigned char[4];
		data[0] = 255;
		data[1] = 255;
		data[2] = 255;
		data[3] = 255;
	}



	//LOG("Width: %d, Height: %d\n", width, height);
	LOG_CALL(glGenTextures, 1, &pMaterial->texId);
	LOG_CALL(glBindTexture, GL_TEXTURE_2D, pMaterial->texId);
	LOG_CALL(glTexImage2D, GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	LOG_CALL(glGenerateMipmap, GL_TEXTURE_2D);
	LOG_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	LOG_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	LOG_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	LOG_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	LOG_CALL(glBindTexture, GL_TEXTURE_2D, 0);
	delete[] data;



	return true;
}

bool Model::loadMeshes(const aiScene* pScene)
{
	bool success = true;
	//LOG("Loading Scene with %d Meshes\n", pScene->mNumMeshes);
	mMeshCount = pScene->mNumMeshes;
	mMeshes = new struct Mesh[mMeshCount];
	for (unsigned int meshIndex = 0; meshIndex < mMeshCount; meshIndex++) {
		loadMesh(pScene, meshIndex);
	}
	return success;
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
