#pragma once
#include "Core/Helpers/ISerializer.h"
#include "spdlog/spdlog.h"
#include <filesystem>
#include <fstream>
#include <memory>
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
        bool loadAsset(const std::string& assetUrl,AssetType& outAsset) const{            
            auto assetPath = getFullPath(assetUrl);
            std::ifstream assetJsonFile(assetPath);
            if(!assetJsonFile){
                spdlog::error("Open file: " + assetPath.generic_string() + " failed!");
                assert(0);
            }
            std::stringstream buffer;
            buffer << assetJsonFile.rdbuf();
            std::string assetJsonText(buffer.str());
            
            std::string errorMessage;
            auto &&assetJson = Json::parse(assetJsonText,errorMessage);
            
            if(!errorMessage.empty()){
                spdlog::error("Parse json file " + assetUrl + " failed!");
                assert(0);
            }
            outAsset.deserialize(assetJson);
            return true;
        }

        template<typename AssetType>
        bool saveAsset(const AssetType& inAsset, const std::string& assetUrl) const{
            auto assetPath = getFullPath(assetUrl);
            std::ofstream assetJsonFile(assetPath);
            if(!assetJsonFile){
                spdlog::error("open file {} failed!", assetUrl);
                assert(0);
            }
            Json assetJson;
            inAsset.serialize(assetJson);
            std::string&& assetJsonText = assetJson.dump();
            assetJsonFile << assetJsonText;
            assetJsonFile.flush();
            return true;
        }
        
        std::filesystem::path getFullPath(const std::string& relativePath) const;
    };
} 
