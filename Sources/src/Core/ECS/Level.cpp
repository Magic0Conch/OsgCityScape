#include <memory>
#include <string>
#include <unordered_map>
#include "Core/ECS/Components/ModelMesh.h"
#include "Core/ECS/Components/Transform.h"
#include "Core/ECS/Object.h"
#include "Core/ECS/ObjectIDAllocator.h"
#include "Core/Helpers/LogSystem.h"
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
    spdlog::info("Level created");
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
            auto transform = &loadedModelObject->getTransformComponent();
            osg::Quat rotation;
            osg::Matrix rotationMatrix(1, 0, 0, 0,
                                        0, 0, -1, 0,
                                        0, 1, 0, 0,
                                        0, 0, 0, 1);
            rotation = rotationMatrix.getRotate();
            transform->setRotation(rotation);
            
            auto mesh = std::make_shared<ModelMesh>(fullPath);
            loadedModelObject->getComponents().push_back(std::make_pair("ModelMesh", mesh));
            std::thread workTread(&ModelMesh::loadResourceAsync,mesh,loadedModelObject);
            workTread.detach();
            std::cout << "Loaded: " << fullPath << std::endl;
        }
    }
    return true;
}

bool Level::load(const std::string& levelResourceUrl){
    //setup scene root
    m_sceneObject = createObjectInLevel("Scene",-1);    
    spdlog::info("Loading level: {}", levelResourceUrl);
    m_levelResourceUrl = levelResourceUrl;
    
    Core::g_runtimeContext.assetManager->loadAsset(levelResourceUrl, *m_levelResource);
    auto rootSceneNode = getRootObject()->getTransformComponent().getNode().get();
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
    auto transform = object->addComponent<Transform>();
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

void Level::setSelectedObjectDirty(bool dirty){
    m_isSelectedObjectDirty = dirty;
}
bool Level::isSelectedObjectDirty() const{
    return m_isSelectedObjectDirty;
}


void Level::printNode(const osg::Node* node, int indent) const{
    auto& log = Core::g_runtimeContext.logSystem;
    std::string msg;
    for(int i = 0; i < indent; i++){
        msg+="  ";
    }
    msg+=(node->getName());
    msg+=(" (Type: ");
    msg+=(node->className());
    msg+=(")");
    msg+=(std::to_string(reinterpret_cast<uintptr_t>(node)));
    log->info(msg);
    const osg::Group* group = node->asGroup();
    if (group) {
        for (unsigned int i = 0; i < group->getNumChildren(); ++i) {
            printNode(group->getChild(i), indent + 1);
        }
    }
}

void Level::printSceneGraph(){
    Core::g_runtimeContext.logSystem->info("Scene Graph:");
    printNode(getRootObject()->getTransformComponent().getNode().get());
}

void Level::addProjectionCamera(std::shared_ptr<ECS::Camera> camera){
    m_cameras.emplace_back(camera);
}
std::vector<std::shared_ptr<Camera>> Level::getCameras() const{
    return m_cameras;
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
    if(selectedObjectID == id)
        return;
    selectedObjectID = id;
    if(id != -1 && id != 0)
        setSelectedObjectDirty(true);
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
    return selectedObjectID != -1 && selectedObjectID != 0;
}

void Level::setLastSelectedObjectID(ObjectID id){
    lastSelectedObjectID = id;
}

ObjectID Level::getLastSelectedObjectID() const{
    return lastSelectedObjectID;
}

std::shared_ptr<CSEditor::ECS::Object> Level::getLastSelectedObject(){
    if(m_objects.find(lastSelectedObjectID) == m_objects.end()){
        spdlog::error("Object not found with id: {}", lastSelectedObjectID);
        return nullptr;
    }
    return m_objects[lastSelectedObjectID];
}

bool Level::hasLastSelectedObject() const{
    return lastSelectedObjectID != -1 && lastSelectedObjectID != 0;
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