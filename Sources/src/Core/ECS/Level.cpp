#include <memory>
#include <string>
#include <unordered_map>
#include "Core/ECS/Components/Mesh.h"
#include "Core/ECS/Components/Transform.h"
#include "Core/ECS/Object.h"
#include "Core/ECS/ObjectIDAllocator.h"
#include "Resources/ResourceType/Common/Object.h"
#include "Editor/Core/RuntimeContext.h"
#include "Resources/ResourceType/Common/Level.h"
#include "Core/ECS/Level.h"
#include "osgDB/FileNameUtils"
#include "osgDB/FileUtils"


using namespace CSEditor::ECS;

Level::Level(){
    m_levelResource = std::make_shared<ResourceType::Level>();
}

/// The main class to manage all game objects
Level::~Level(){

}


bool Level::importObjFromFolderRecursively(const std::string& folderPath){
    osgDB::DirectoryContents contents = osgDB::getDirectoryContents(folderPath);

    for (const auto& file : contents) {
        std::string fullPath = osgDB::concatPaths(folderPath, file);
        if (osgDB::fileType(fullPath) == osgDB::DIRECTORY && file!="." && file!="..") {
            importObjFromFolderRecursively(fullPath);
        } else if (osgDB::getLowerCaseFileExtension(fullPath) == "ive") {
            if (fullPath.find("_bbox") != std::string::npos) {
                continue;
            }
            auto loadedModelObject = createObjectInLevel(file, m_sceneObject->getID());
            auto mesh = loadedModelObject->addComponent<Mesh>();
            mesh->m_meshPath = fullPath;            
            std::thread workTread(&Mesh::loadResource,mesh,loadedModelObject);
            workTread.detach();
            std::cout << "Loaded: " << fullPath << std::endl;
        }
    }

    return true;
}

bool Level::load(const std::string& levelResourceUrl){
    //setup scene root
    m_sceneObject = createObjectInLevel("Scene",-1);


    // auto loadedModelObject = createObjectInLevel("cow", m_sceneObject->getID());
    // auto cowMesh = loadedModelObject->addComponent<Mesh>();
    // cowMesh->setMeshPath("resources/models/cow.osg");
    // cowMesh->loadResource(loadedModelObject);
    //load level resource
    
    spdlog::info("Loading level: {}", levelResourceUrl);
    m_levelResourceUrl = levelResourceUrl;
    
    Core::g_runtimeContext.assetManager->loadAsset(levelResourceUrl, *m_levelResource);
    return true;
}

ObjectID Level::loadObjectInstance(const ResourceType::ObjectInstance& objectInstance){
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

void Level::setIsLoaded(bool isLoaded){
    m_isLoaded = isLoaded;
}

bool Level::getIsLoaded() const{
    return m_isLoaded;
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

std::shared_ptr<Object> Level::createObjectInLevel(const std::string& name,const ObjectID& parentID){
    auto objectId = ObjectIDAllocator::alloc();
    auto object = std::make_shared<ECS::Object>(objectId,parentID);

    std::shared_ptr<Transform> transform = std::make_shared<Transform>();
    transform->loadResource(object);
    object->setTransform(transform);
    object->setName(name);
    m_objects[objectId] = object;
    if (parentID == -1) {
        return getSceneObjectById(objectId);
    }
    // auto transform = object->addComponent<Transform>();
    // transform->loadResource(object);
    // object->setName(name);
    // m_objects[objectId] = object;
    
    const auto& parentObject = getSceneObjectById(parentID);
    parentObject->getTransformComponent().addChild(*transform);
    return object;
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

std::shared_ptr<CSEditor::ResourceType::Level> Level::getLevelResource(){
    return m_levelResource;
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