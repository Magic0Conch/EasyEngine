#include "PathUtility.h"
#include <runtime/core/base/macro.h>
namespace EasyEngine{
    std::string PathUtility::getRootPath(const std::string& path){
        const std::string platform = getPlatformName();
        int index = path.find_last_of('/');
        if (platform == "windows") {
            index = path.find_last_of('\\');
        }
        std::string rootPath = path.substr(0,index+1);
        
        return rootPath;
    }

    std::string PathUtility::getFullPath(const std::string &directory, const std::string &filePath){
        const std::string platform = getPlatformName();
        std::string fullPath;
        fullPath = directory;
        if (platform == "windows") {
            if (directory.back()!='\\') {
                fullPath.push_back('\\');
            }
        }
        else if (platform == "linux") {
            if (directory.back()!='/') {
                fullPath.push_back('/');
            }
        }
        fullPath.append(filePath);
        return fullPath;
    }
}