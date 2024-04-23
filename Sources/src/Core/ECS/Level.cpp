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
#include "osg/Matrix"
#include "osg/Vec3f"
#include "osgDB/FileNameUtils"
#include "osgDB/FileUtils"


using namespace CSEditor::ECS;

Level::Level(){
    m_levelResource = std::make_shared<ResourceType::Level>();
}

/// The main class to manage all game objects
Level::~Level(){

}


bool Level::importObjFromFolderRecursively(const std::string& folderPath,const std::string& extension){
    osgDB::DirectoryContents contents = osgDB::getDirectoryContents(folderPath);

    for (const auto& file : contents) {
        std::string fullPath = osgDB::concatPaths(folderPath, file);
        if (osgDB::fileType(fullPath) == osgDB::DIRECTORY && file!="." && file!="..") {
            importObjFromFolderRecursively(fullPath);
        } else if (osgDB::getLowerCaseFileExtension(fullPath) == extension) {
            if (fullPath.find("_bbox") != std::string::npos) {
                continue;
            }
            auto loadedModelObject = createObjectInLevel(file, m_sceneObject->getID());
            auto pat = loadedModelObject->getTransformComponent().getNode();
            osg::Quat rotation;
            osg::Matrix rotationMatrix(1, 0, 0, 0,
                                        0, 0, -1, 0,
                                        0, 1, 0, 0,
                                        0, 0, 0, 1);
            rotation = rotationMatrix.getRotate();
            pat->setAttitude(rotation);
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
    auto rootSceneNode = getRootObject()->getTransformComponent().getNode().get();
    rootSceneNode->setNodeMask(0x1);
    Core::g_runtimeContext.viewer->setSceneData(rootSceneNode);
    auto objects = m_levelResource->getObjects();
    for (const ResourceType::ObjectInstance& objectInstanceRes:objects) {
        auto curObjectId = loadObjectInstance(objectInstanceRes);
        auto thisTransform = getSceneObjectById(curObjectId)->getTransformComponent();
        getRootObject()->addChild(thisTransform);
        registerNode2IDMap(thisTransform.getNode().get(), curObjectId);
    }
    setIsLoaded(true);

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
    
    const auto& parentObject = getSceneObjectById(parentID);
    parentObject->getTransformComponent().addChild(*transform);
    return object;
}

void Level::registerNode2IDMap(osg::Node* node,ECS::ObjectID id){
    nodeToObjectID[node] = id;
    osg::Group* group = node->asGroup();
    if (group) {
        for (unsigned int i = 0; i < group->getNumChildren(); ++i) {
            registerNode2IDMap(group->getChild(i), id);
        }
    }
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

void Level::setSelectedObjectID(ObjectID id){    
    selectedObjectID = id;
}

ObjectID Level::getSelectedObjectID() const{
    return selectedObjectID;
}

std::shared_ptr<CSEditor::ECS::Object> Level::getSelectedObject(){
    if(m_objects.find(selectedObjectID) == m_objects.end()){
        spdlog::error("Object not found with id: {}", selectedObjectID);
        return nullptr;
    }
    return m_objects[selectedObjectID];
}

bool Level::isObjectSelected() const{
    return selectedObjectID != -1;
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