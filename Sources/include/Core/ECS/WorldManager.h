#pragma once
#include "Core/ECS/Level.h"
#include "Resources/ResourceType/Common/World.h"
#include "Resources/ResourceType/Common/Level.h"
#include <memory>
#include <string>
#include <unordered_map>
namespace CSEditor::ECS{

class WorldManager{
public:
    virtual ~WorldManager();
    WorldManager();

    void initialize();
    void clear();

    void reloadCurrentLevel();
    void saveCurrentLevel();

    void tick(float delta_time);
    std::shared_ptr<ECS::Level> getCurrentActiveLevel() const;

private:
    bool loadLevel(const std::string& level_url);
    bool loadWorld(const std::string& world_url);

    bool m_isWorldLoaded {false};
    std::string m_currentWorldUrl;
    // std::shared_ptr<WorldRes> m_current_world_resource;

    // all loaded levels, key: level url, vaule: level instance
    std::unordered_map<std::string, std::shared_ptr<ECS::Level>> m_loadedLevels;
    // active level, currently we just support one active level
    std::shared_ptr<ECS::Level> m_currentActiveLevel;
    std::shared_ptr<ResourceType::World> m_currentWorldResource;
};

}