#include "Texture.h"
#include <opencv2/imgcodecs.hpp>
#include <string>

namespace EasyEngine {
    Texture::Texture(const std::string& path,const GLenum& textureType,bool gammaCorrection,bool inverseY){
        cv::Mat data = ImageProcessing::readImageByPath(path,IMREAD_UNCHANGED,inverseY);
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
                format_target = GL_RED;//internalFormat 
                format_source = GL_RED;
                break;
            case 3:
                format_target = gammaCorrection?GL_SRGB:GL_RGB;
                format_source = GL_BGR;
                break;
            case 4:
                format_target = gammaCorrection?GL_SRGB_ALPHA:GL_RGBA;
                format_source = GL_BGRA;
                break;
        } 
        glTexImage2D(GL_TEXTURE_2D,0,format_target,data.cols,data.rows,0,format_source,GL_UNSIGNED_BYTE,data.data);
        glGenerateMipmap(GL_TEXTURE_2D);
        data.release();
    }

    Texture::Texture(const std::vector<std::string>& paths,const GLenum& textureType){
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_CUBE_MAP, id);
        int t = 0;
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); 
        for (const std::string& path:paths) {
            cv::Mat data = ImageProcessing::readImageByPath(path);
            if (data.empty()) {
                cout << "Texture::Texture: load texture from "+path + " failed";
                return;
            }
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
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + t++,0,format_target,data.cols,data.rows,0,format_source,GL_UNSIGNED_BYTE,data.data);
            data.release();
        }
    }


}