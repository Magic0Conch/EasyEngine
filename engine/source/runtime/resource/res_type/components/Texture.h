#pragma once
#include <string>
#include "runtime/include/ImageProcessing.h"
#include <string>
#include <glad/glad.h>

namespace EasyEngine{
class Texture{
private:
public:
    unsigned int id;
    std::string type;
    std::string path;
    Texture(const std::string& path);
};

}