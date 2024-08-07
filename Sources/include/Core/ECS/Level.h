#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "Core/ECS/Components/Camera.h"
#include "Core/ECS/Object.h"
#include "ObjectIDAllocator.h"
#include "Core/ECS/Level.h"
#include "Resources/ResourceType/Common/Level.h"
#include "Resources/ResourceType/Common/Object.h"
#include "Object.h"
#include "osg/Group"
#include "osg/Node"
#include "osg/ref_ptr"
namespace CSEditor::ECS{

class GObject;

// using LevelObjectsMap = std::unordered_map<GObjectID, std::shared_ptr<GObject>>;

class Level
{
public:

    Level();
    virtual ~Level();

    bool load(const std::string& levelResourceUrl);
    void unload();

    bool save();

    void tick(float delta_time);

    const std::string& getLevelResUrl() const;

    void buildSceneGraph();

    ObjectID loadObjectInstance(const ResourceType::ObjectInstance& objectInstance);

    std::shared_ptr<Object> getRootObject();
    std::shared_ptr<Object> getSceneObjectById(const ObjectID& objectID);
    std::unordered_map<ObjectID, std::shared_ptr<Object>>& getSceneObjectsMap();

    void setIsLoaded(bool isLoaded);
    bool getIsLoaded() const;


    std::shared_ptr<ResourceType::Level> getLevelResource();
    // const LevelObjectsMap& getAllGObjects() const;

    // std::weak_ptr<GObject> getGObjectByID(GObjectID go_id) const;
    bool importObjFromFolderRecursively(const std::string& folderPath,const std::string& extension="ive");


    // GObjectID createObject(const ObjectInstanceRes& object_instance_res);
    // void deleteGObjectByID(GObjectID go_id);

    /*create empty object in the scene as the child of parentID*/
    std::shared_ptr<Object> createObjectInLevel(const std::string& name,const ObjectID& parentID);
    void setSelectedObjectID(ObjectID id);
    ObjectID getSelectedObjectID() const;
    std::shared_ptr<ECS::Object> getSelectedObject();
    bool isObjectSelected() const;
    void setLastSelectedObjectID(ObjectID id);
    ObjectID getLastSelectedObjectID() const;
    std::shared_ptr<ECS::Object> getLastSelectedObject();
    std::unordered_map<osg::Node*, ObjectID> nodeToObjectID;
    bool hasLastSelectedObject() const;
    void setSelectedObjectDirty(bool dirty);
    bool isSelectedObjectDirty() const;
    
    void addProjectionCamera(std::shared_ptr<ECS::Camera> camera);
    std::vector<std::shared_ptr<ECS::Camera>> getCameras() const;

    void printNode(const osg::Node* node, int indent = 0) const;
    void printSceneGraph();
    void registerNode2IDMap(osg::Node* node,ECS::ObjectID id);
protected:
    void clear();

    bool m_isLoaded {false};
    std::string m_levelResourceUrl;

    std::unordered_map<ObjectID, std::shared_ptr<Object>> m_objects;
    std::vector<std::shared_ptr<ECS::Camera>> m_cameras;
    std::shared_ptr<Object> m_sceneObject;
    std::shared_ptr<ResourceType::Level> m_levelResource;
private:
    ObjectID selectedObjectID = -1;
    ObjectID lastSelectedObjectID = -1;

    std::shared_ptr<Object> m_selectedObject = nullptr;
    bool m_isSelectedObjectDirty = false;
};

}