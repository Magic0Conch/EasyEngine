#include "../include/Texture.h"
// namespace EasyEngine {
// Texture::Texture() {
// 	glGenTextures(1, &ID);
// }

// Texture::Texture(const string& texturePath, int sourceType, int targetType, int textureUnit) {
// 	glGenTextures(1, &ID);
// 	glBindTexture(GL_TEXTURE_2D, ID);
// 	//set the texture wrapping/filtering options (on the currently bound texture object)
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
// 	setTextureMat(texturePath, sourceType, targetType);
// }

// void Texture::setTextureMat(const string texturePath, const int sourceType, const int targetType) {
// 	glBindTexture(GL_TEXTURE_2D, ID);
// 	textureMat = ImageProcessing::readImageByPath(texturePath);
// 	if (!textureMat.empty()) {
// 		//the second argument specify the mipmap level, the seventh argument specify the format and datatype of the texture image.
// 		glTexImage2D(GL_TEXTURE_2D, 0, targetType, textureMat.cols, textureMat.rows, 0, sourceType, GL_UNSIGNED_BYTE, textureMat.data);
// 		glGenerateMipmap(GL_TEXTURE_2D);
// 	}
// 	else {
// 		cout << "Failed to load Texture: "<<texturePath << endl;
// 	}

// }

// void Texture::setTextureUnit(int textureUnit) {
// 	glActiveTexture(GL_TEXTURE0 + textureUnit);
// 	glBindTexture(GL_TEXTURE_2D, ID);
// }
// }