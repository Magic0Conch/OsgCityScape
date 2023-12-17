#pragma once

#include "spdlog/spdlog.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <json11.hpp>

namespace CSEditor::Resources
{
    using Json = json11::Json;
    class AssetManager
    {
    public:
        template<class AssetType>
        bool loadAsset(const std::string& assetUrl, AssetType& outAsset) const{
            auto assetPath = getFullPath(assetUrl);
            std::ifstream assetJsonFile(assetPath);
            if(!assetJsonFile){
                spdlog::error("Open file: " + assetPath.generic_string() + " failed!");
                return false;
            }
            std::stringstream buffer;
            buffer << assetJsonFile.rdbuf();
            std::string assetJsonText(buffer.str());

            std::string errorMessage;
            auto &&assetJson = Json::parse(assetJsonText,errorMessage);
            
        }

        std::filesystem::path getFullPath(const std::string& relativePath) const;
    };
} 
