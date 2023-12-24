#include "Resources/ResourceType/Common/World.h"

using namespace CSEditor::ResourceType;

const std::string& World::getName() const {
    return m_name;
}

void World::setName(const std::string& name) {
    m_name = name;
}

const std::vector<std::string>& World::getLevelUrls() const {
    return m_levelUrls;
}

void World::setLevelUrls(const std::vector<std::string>& levelUrls) {
    m_levelUrls = levelUrls;
}

const std::string& World::getDefaultLevelUrl() const {
    return m_defaultLevelUrl;
}

void World::setDefaultLevelUrl(const std::string& defaultLevelUrl) {
    m_defaultLevelUrl = defaultLevelUrl;
}

void World::serialize(Json& jsonBody){
    Json::object root = Json::object{}; 
    root["level_urls"] = getLevelUrls();
    root["name"] = getName();
    root["default_level_url"] = getDefaultLevelUrl();        
}

void World::deserialize(Json& jsonBody){
    const auto& defaultLevelUrl = jsonBody["default_level_url"].string_value();
    setDefaultLevelUrl(defaultLevelUrl);
    const auto& name = jsonBody["name"].string_value();
    setName(name);
    const auto& levelUrlJsons = jsonBody["level_urls"].array_items();        
    std::vector<std::string> levelUrls;
    for (auto& levelUrlJson: levelUrlJsons) {
        levelUrls.emplace_back(levelUrlJson.string_value());
    }
    setLevelUrls(levelUrls);
}