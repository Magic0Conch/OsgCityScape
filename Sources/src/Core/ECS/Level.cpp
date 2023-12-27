#include <memory>
#include <string>
#include <unordered_map>
#include "Core/ECS/ObjectIDAllocator.h"
#include "Core/ECS/Level.h"
#include "Editor/Core/RuntimeContext.h"
#include "Resources/ResourceType/Common/Level.h"
#include "spdlog/spdlog.h"

using namespace CSEditor::ECS;


/// The main class to manage all game objects
Level::~Level(){

}

bool Level::load(const std::string& levelResourceUrl){
    spdlog::info("Loading level: {}", levelResourceUrl);
    m_levelResourceUrl = levelResourceUrl;
    ResourceType::Level levelResource;
    const bool isLoadedSuccess = Core::g_runtimeContext.assetManager->loadAsset(levelResourceUrl, levelResource);
    
    return true;
}
// void Level::unload(){

// }

// bool Level::save(){
//     return true;
// }

// void Level::tick(float delta_time){

// }

// const std::string& Level::getLevelResUrl() const { 
//     return m_level_res_url; 
// }

// const LevelObjectsMap& Level::getAllGObjects() const { 
//     return m_gobjects; 
// }

// std::weak_ptr<GObject> Level::getGObjectByID(GObjectID go_id) const{
//     std::weak_ptr<GObject> gObject; 
//     return gObject;
// }


// GObjectID Level::createObject(const ObjectInstanceRes& object_instance_res){
//     GObjectID result;
//     return result;
// }
// void Level::deleteGObjectByID(GObjectID go_id){

// }

// void Level::clear(){

// }