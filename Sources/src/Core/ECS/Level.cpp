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
#include "osgGA/TrackballManipulator"


using namespace CSEditor::ECS;


/// The main class to manage all game objects
Level::~Level(){

}

bool Level::load(const std::string& levelResourceUrl){
    //setup scene root
    auto objectId = ObjectIDAllocator::alloc();
    Transform* rootTransform = new Transform();
    m_sceneObject = std::make_shared<Object>(objectId,-1);
    m_sceneObject->setTransform(rootTransform);
    m_sceneObject->setName("Scene");
    m_objects[objectId] = m_sceneObject;
    auto viewer = Core::g_runtimeContext.viewer;
    viewer->setSceneData(rootTransform->getNode().get());

    osg::ref_ptr<osgGA::TrackballManipulator> manipulator = new osgGA::TrackballManipulator();
    manipulator->setHomePosition(osg::Vec3d(0,10,0), osg::Vec3d(0,0,0),osg::Vec3f(0,0,1));
    viewer->setCameraManipulator(manipulator);

    //load level resource
    spdlog::info("Loading level: {}", levelResourceUrl);
    m_levelResourceUrl = levelResourceUrl;
    ResourceType::Level levelResource;
    Core::g_runtimeContext.assetManager->loadAsset(levelResourceUrl, levelResource);
    auto objects = levelResource.getObjects();
    for (const ResourceType::ObjectInstance& objectInstanceRes:objects) {
        auto curObjectId = createObject(objectInstanceRes);
        auto thisTransform = m_objects[curObjectId]->getTransformComponent();
        m_sceneObject->addChild(thisTransform);
    }
    m_isLoaded = true;
    return true;
}

ObjectID Level::createObject(const ResourceType::ObjectInstance& objectInstance){
    auto objectId = ObjectIDAllocator::alloc();
    Transform* transform = new Transform();
    std::shared_ptr<ECS::Object> object = std::make_shared<ECS::Object>(objectId,m_sceneObject->getID());
    object->setTransform(transform);
    bool isLoaded = object->load(objectInstance);
    if(isLoaded){
        m_objects[objectId] = object;
    }
    return objectId;
}


void Level::buildSceneGraph(){

    for (auto&[objectId,objectPtr] :m_objects) {
        
    }
    // auto objects = levelResource.getObjects();


    // Core::g_runtimeContext.viewer.set
}

std::shared_ptr<Object> Level::getRootObject(){
    return m_sceneObject;
}

std::shared_ptr<Object> Level::getSceneObjectById(const ObjectID& objectID){
    return m_objects[objectID];
}

std::unordered_map<ObjectID, std::shared_ptr<Object>>& Level::getSceneObjectsMap(){
    return m_objects;
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