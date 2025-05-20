#pragma once
#include "Core/Helpers/ISerializer.h"
#include "Core/Helpers/Serializer.h"
#include <osgDB/ReadFile>
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
        Json loadJson(const std::string& jsonUrl) const{
            auto assetPath = getFullPath(jsonUrl);
            std::ifstream assetJsonFile(assetPath);
            if(!assetJsonFile){
                spdlog::error("Open file: " + jsonUrl + " failed!");
                spdlog::error("Open file: " + assetPath.generic_string() + " failed!");
                assert(0);
            }
            std::stringstream buffer;
            buffer << assetJsonFile.rdbuf();
            std::string assetJsonText(buffer.str());
            
            std::string errorMessage;
            auto assetJson = Json::parse(assetJsonText,errorMessage);
            if(!errorMessage.empty()){
                spdlog::error("Parse json file " + jsonUrl + " failed!");
                assert(0);
            }
            return assetJson;
        }

        template<class AssetType>
        bool loadAsset(const std::string& assetUrl,AssetType& outAsset) const{            
            auto &&assetJson = loadJson(assetUrl);
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

        template<typename ResourceType>
        bool deserialize(const std::string& resourceUrl,ResourceType& outResource) const{
            auto &&resourceJson = loadJson(resourceUrl);
            outResource.deserialize(resourceJson);
            return true;
        }
        
        std::filesystem::path getFullPath(const std::string& relativePath) const;

        osg::ref_ptr<osg::Image> loadImage(const std::string& fileName) {
            osg::ref_ptr<osg::Image> image = osgDB::readImageFile(fileName);
            if (!image) {
                std::cerr << "Error: Failed to load image: " << fileName << std::endl;
            } else {
                std::cout << "Successfully loaded image: " << fileName << std::endl;
            }
            return image;
        }

    };
} 
