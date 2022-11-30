#include "include/ModelMesh.h"
ModelMesh::ModelMesh() {
	this->meshDataSize = 0;
	this->indices = NULL;
	this->meshData = NULL;

}

ModelMesh::ModelMesh(const std::string& meshDataFilePath) {
	this->meshData = Utility::floatArrayRead(meshDataFilePath,meshDataSize);
	this->meshDataSize = meshDataSize * sizeof(float);
	indices = NULL;
}

ModelMesh::~ModelMesh() {
	if (meshData != NULL) {
		delete[] meshData;
	}
	if (indices != NULL) {
		delete[] indices;
	}
}

ModelMesh::ModelMesh(const std::string& meshDataFilePath, const std::string& indicesFilePath) {
	this->meshData = Utility::floatArrayRead(meshDataFilePath, meshDataSize);
	this->meshDataSize = meshDataSize * sizeof(float);
	this->indices = NULL;
	setIndicies(indicesFilePath);
}

ModelMesh::ModelMesh(const std::string& meshDataFilePath, const string* texturePath, const int textureLength, const std::string& indicesFilePath) {
	this->meshData = Utility::floatArrayRead(meshDataFilePath, meshDataSize);
	this->meshDataSize = meshDataSize * sizeof(float);
	setIndicies(indicesFilePath);	
	setTextures(textureLength, texturePath);
}
ModelMesh::ModelMesh(const std::string& meshDataFilePath, const string* texturePath, const int textureLength) {
	this->meshData = Utility::floatArrayRead(meshDataFilePath, meshDataSize);
	this->meshDataSize = meshDataSize * sizeof(float);
	this->indices = NULL;
	setTextures(textureLength, texturePath);
}

//ModelMesh::ModelMesh(const std::string& meshDataFilePath, const std::string& indicesFilePath, const int textureLength) {
//	this->meshData = Utility::floatArrayRead(meshDataFilePath, meshDataSize);
//	this->meshDataSize = meshDataSize * sizeof(float);
//	setIndicies(indicesFilePath);
//	this->textureLength = textureLength;
//	this->textures = new Texture[textureLength];
//}
ModelMesh::ModelMesh(const std::string& meshDataFilePath, const int textureLength) {
	this->meshData = Utility::floatArrayRead(meshDataFilePath, meshDataSize);
	this->meshDataSize = meshDataSize * sizeof(float);
	this->indices = NULL;
	this->textureLength = textureLength;
	this->textures = new Texture[textureLength];
}
ModelMesh::ModelMesh(const int textureLength) {
	this->textureLength = textureLength;
	this->textures = new Texture[textureLength];
}

void ModelMesh::setIndicies(const std::string& indicesFilePath) {
	this->indices = Utility::intArrayRead(indicesFilePath, indicesSize);
	this->indicesSize = indicesSize * sizeof(int);
}

void ModelMesh::setTextures(const int textureLength, const string* texturePath, int sourceType, int targetType) {
	this->textureLength = textureLength;
	this->textures = new Texture[textureLength];
	for (int i = 0; i < textureLength; ++i) {
		this->textures[i].setTextureMat(texturePath[i], sourceType, targetType);
	}
}