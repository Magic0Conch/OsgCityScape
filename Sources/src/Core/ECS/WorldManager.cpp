#include "Core/ECS/WorldManager.h"
#include "Resources/ResourceType/Common/Level.h"
#include "Resources/ResourceType/Common/World.h"
#include "spdlog/spdlog.h"
#include "Resources/ResourceManagement/ConfigManager.h"
#include "Editor/Core/RuntimeContext.h"
#include <memory>
#include <string>
#include <typeinfo>
#include <unordered_map>
namespace CSEditor::ECS{

WorldManager::WorldManager(){
    clear(); 
    initialize();
}

WorldManager::~WorldManager() { 
}

void WorldManager::initialize(){
    m_isWorldLoaded   = false;
    m_currentWorldUrl = Core::g_runtimeContext.configManager->getDefaultWorldUrl().string();
    loadWorld(m_currentWorldUrl);
}

void WorldManager::clear()
{
    // unload all loaded levels
    for (auto level_pair : m_loadedLevels)
    {
        // level_pair.second->unload();
    }
    m_loadedLevels.clear();

    m_currentActiveLevel.reset();

    // clear world
    // m_current_world_resource.reset();
    m_currentWorldUrl.clear();
    m_isWorldLoaded = false;
}

void WorldManager::tick(float delta_time)
{
    if (!m_isWorldLoaded){
        loadWorld(m_currentWorldUrl);
    }

    // tick the active level
    std::shared_ptr<ECS::Level> activeLevel = m_currentActiveLevel;
    if (activeLevel){
        // active_level->tick(delta_time);
    }
}

bool WorldManager::loadWorld(const std::string& world_url)
{
    spdlog::info("Loading world from " + world_url + ".");
    ResourceType::World worldRes;
    const auto isWorldLoadSuccess = Core::g_runtimeContext.assetManager->loadAsset(world_url,worldRes);
    if(!isWorldLoadSuccess){
        return false;
    }
    m_currentWorldResource = std::make_shared<ResourceType::World>(worldRes);

    const bool isLevelLoadSuccess = loadLevel(worldRes.getDefaultLevelUrl());
    if(!isLevelLoadSuccess){
        return false;
    }
    // set the default level to be active level
    auto iter = m_loadedLevels.find(worldRes.getDefaultLevelUrl());
    assert(iter != m_loadedLevels.end());
    m_currentActiveLevel = iter->second;

    m_isWorldLoaded = true;
    spdlog::info("World load succeed!");
    return true;
}

bool WorldManager::loadLevel(const std::string& levelUrl)
{
    // std::shared_ptr<ECS::Level> level = std::make_shared<ECS::Level>();
    m_currentActiveLevel = std::make_shared<Level>();

    const bool isLevelLoadSuccess = m_currentActiveLevel->load(levelUrl);
    if (!isLevelLoadSuccess){
        return false;
    }

    m_loadedLevels[levelUrl] = m_currentActiveLevel;
    return true;
}

void WorldManager::reloadCurrentLevel(){
    auto active_level = m_currentActiveLevel;
    if (active_level == nullptr)
    {
        // LOG_WARN("current level is nil");
        return;
    }

    // const std::string level_url = active_level->getLevelResUrl();
    // active_level->unload();
    // m_loaded_levels.erase(level_url);

    // const bool is_load_success = loadLevel(level_url);
    // if (!is_load_success)
    {
        // LOG_ERROR("load level failed {}", level_url);
        return;
    }

    // update the active level instance
    // auto iter = m_loaded_levels.find(level_url);
    // ASSERT(iter != m_loaded_levels.end());

    // m_current_active_level = iter->second;

    // LOG_INFO("reload current evel succeed");
}

void WorldManager::saveCurrentLevel()
{
    auto active_level = m_currentActiveLevel;

    if (active_level == nullptr)
    {
        // LOG_ERROR("save level failed, no active level");
        return;
    }

    // active_level->save();
}

std::shared_ptr<ECS::Level> WorldManager::getCurrentActiveLevel() const{
    return m_currentActiveLevel; 
}


}