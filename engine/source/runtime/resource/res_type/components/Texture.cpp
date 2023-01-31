#include "Texture.h"

namespace EasyEngine {
    Texture::Texture(const std::string& path){
        cv::Mat data = ImageProcessing::readImageByPath(path);
        if (data.empty()) {
            cout << "Texture::Texture: load texture from "+path + " failed";
            return;
        }
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        //set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        GLenum format_target,format_source;
        switch (data.channels()) {
            case 1:
                format_target = GL_RED;
                format_source = GL_RED;
                break;
            case 3:
                format_target = GL_RGB;
                format_source = GL_BGR;
                break;
            case 4:
                format_target = GL_RGBA;
                format_source = GL_BGRA;
                break;
        } 
        glTexImage2D(GL_TEXTURE_2D,0,format_target,data.cols,data.rows,0,format_source,GL_UNSIGNED_BYTE,data.data);
        glGenerateMipmap(GL_TEXTURE_2D);
        data.release();
    }

}