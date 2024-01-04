#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "Core/ECS/Object.h"
#include "Core/ECS/ObjectIDAllocator.h"
#include "Resources/ResourceType/Common/Object.h"
#include "Editor/Core/RuntimeContext.h"
#include "Resources/ResourceType/Common/Level.h"
#include "Core/ECS/Level.h"


using namespace CSEditor::ECS;


/// The main class to manage all game objects
Level::~Level(){

}

bool Level::load(const std::string& levelResourceUrl){
    spdlog::info("Loading level: {}", levelResourceUrl);
    m_levelResourceUrl = levelResourceUrl;
    ResourceType::Level levelResource;
    const bool isLoadedSuccess = Core::g_runtimeContext.assetManager->loadAsset(levelResourceUrl, levelResource);
    auto objects = levelResource.getObjects();
    for (const ResourceType::ObjectInstance& objectInstanceRes:objects) {
        createObject(objectInstanceRes);
    }
    m_isLoaded = true;
    return true;
}

ObjectID Level::createObject(const ResourceType::ObjectInstance& objectInstance){
    auto objectId = ObjectIDAllocator::alloc();
    std::shared_ptr<ECS::Object> object = std::make_shared<ECS::Object>(objectId);
    bool isLoaded = object->load(objectInstance);
    if(isLoaded){
        m_gobjects[objectId] = object;
    }
    return objectId;
}


void Level::buildSceneGraph(){
    osg::ref_ptr<osg::Group> root = new osg::Group;
    // auto objects = levelResource.getObjects();


    // Core::g_runtimeContext.viewer.set
}


// void Level::unload(){

// }

// bool Level::save(){
//     return true;
// }

// void Level::tick(float delta_time){

// }

const std::string& Level::getLevelResUrl() const { 
    return m_levelResourceUrl; 
}

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