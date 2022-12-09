#pragma once
#include <string>
// #include <filesystem>
#include "../../function/global/global_context.h"
#include <fstream>
#include "../config_manager/ConfigManager.h"
namespace EasyEngine {
    class GlobalContext;
    class ConfigManager;

    class AssetManager{
    public:
        // template<typename AssetType>
        // bool loadAsset(const std::string&,AssetType&) const;
        // template<typename AssetType>
        // bool saveAsset(const AssetType&,std::string&) const;
        const std::string getFullPath(const std::string& relative_path) const;

    };
}