#pragma once
#include <string>
namespace EasyEngine {
    class PathUtility{        
    public:
        static std::string getRootPath(const std::string&);
        static std::string getFullPath(const std::string& directory,const std::string& filePath);
    };
}