#pragma once
#include <string>
#include "Utility.h"
#include "Texture.h"
class ModelMesh
{
public:
	float* meshData = NULL;
	int* indices = NULL;
	int meshDataSize = 0;
	int indicesSize = 0;
	int textureLength = 0;
public:
	Texture* textures;
public:
	ModelMesh();
	ModelMesh(const std::string&);
	ModelMesh(const std::string&, const std::string&);
	ModelMesh(const std::string&, const string*, const int, const std::string&);
	ModelMesh(const std::string&, const string*, const int textureLength = 1);
	//ModelMesh(const std::string&, const std::string& ,const int textureLength = 1);
	ModelMesh(const std::string&, const int textureLength = 1);
	ModelMesh(const int textureLength = 1);
	~ModelMesh();
	void setIndicies(const std::string& indicesFilePath);
	void setTextures(const int textureLength, const string* texturePath, int sourceType = GL_BGR, int targetType = GL_RGB);
};

