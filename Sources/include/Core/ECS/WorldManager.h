#pragma once
#include "Level.h"
#include <memory>
#include <string>
#include <unordered_map>
namespace CSEditor::Core{

class WorldManager{
public:
    virtual ~WorldManager();

    void initialize();
    void clear();

    void reloadCurrentLevel();
    void saveCurrentLevel();

    void tick(float delta_time);
    std::weak_ptr<Level> getCurrentActiveLevel() const { return m_current_active_level; }

private:
    bool loadWorld(const std::string& world_url);
    bool loadLevel(const std::string& level_url);

    bool m_is_world_loaded {false};
    std::string m_current_world_url;
    // std::shared_ptr<WorldRes> m_current_world_resource;

    // all loaded levels, key: level url, vaule: level instance
    std::unordered_map<std::string, std::shared_ptr<Level>> m_loaded_levels;
    // active level, currently we just support one active level
    std::weak_ptr<Level> m_current_active_level;

};

}