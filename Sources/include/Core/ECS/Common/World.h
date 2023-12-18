#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include "Core/Helpers/ISerializer.h"
#include "json11.hpp"

namespace CSEditor::ECS{
class World: public Helpers::ISerializable {
public:
    virtual void serialize(Json& jsonBody) override{
        Json::object root = Json::object{}; 
        root["level_urls"] = level_urls;
        root["name"] = name;
        root["default_level_url"] = default_level_url;
        
    }
    virtual void deserialize(Json& jsonBody) override{
        default_level_url = jsonBody["default_level_url"].string_value();
        name = jsonBody["name"].string_value();
        auto levelUrlJsons = jsonBody["level_urls"].array_items();        
        level_urls.clear();
        for (auto& levelUrlJson: levelUrlJsons) {
            level_urls.emplace_back(levelUrlJson.string_value());
        }
    }
public:
    std::string name;
    std::vector<std::string> level_urls;
    std::string default_level_url;
};
}