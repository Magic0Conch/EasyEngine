#include "PathUtility.h"
namespace EasyEngine{
    std::string PathUtility::getRootPath(const std::string& path){
        int index = path.find_last_of('/');
        std::string rootPath = path.substr(0,index+1);
        return rootPath;
    }

    std::string PathUtility::getFullPath(const std::string &directory, const std::string &filePath){
        std::string fullPath;
        fullPath = directory;
        if (directory.back()!='/') {
            fullPath.push_back('/');
        }
        fullPath.append(filePath);
        return fullPath;
    }
}