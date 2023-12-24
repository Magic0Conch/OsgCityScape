#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include "Core/Helpers/ISerializer.h"
#include "json11.hpp"

namespace CSEditor::ResourceType{
class World: public Helpers::ISerializable {
public:
    virtual void serialize(Json& jsonBody) override;

    virtual void deserialize(Json& jsonBody) override;

    const std::string& getName() const;
    void setName(const std::string& name);

    const std::vector<std::string>& getLevelUrls() const;
    void setLevelUrls(const std::vector<std::string>& levelUrls);

    const std::string& getDefaultLevelUrl() const;
    void setDefaultLevelUrl(const std::string& defaultLevelUrl);

public:
    std::string m_name;
    std::vector<std::string> m_levelUrls;
    std::string m_defaultLevelUrl;
};
}