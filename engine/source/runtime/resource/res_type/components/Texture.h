#pragma once
#include <string>
#include "runtime/include/ImageProcessing.h"
#include <string>
#include <glad/glad.h>
#include <vector>

namespace EasyEngine{
class Texture{
private:
public:
    unsigned int id;
    std::string type;
    std::string path;
    Texture(const std::string& path,const GLenum& textureType = GL_TEXTURE_2D,bool gammaCorrection = false,bool inverseY = false);
    Texture(const std::vector<std::string>& paths,const GLenum& textureType = GL_TEXTURE_CUBE_MAP);
};

}