#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include "Core/ECS/Object.h"
#include "ObjectIDAllocator.h"
#include "Core/ECS/Level.h"
#include "Resources/ResourceType/Common/Level.h"
#include "Resources/ResourceType/Common/Object.h"
#include "Object.h"
#include "osg/Group"
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
    

    // GObjectID createObject(const ObjectInstanceRes& object_instance_res);
    // void deleteGObjectByID(GObjectID go_id);
    
    /*create empty object in the scene as the child of parentID*/
    std::shared_ptr<Object> createObjectInLevel(const std::string& name,const ObjectID& parentID);

protected:
    void clear();

    bool m_isLoaded {false};
    std::string m_levelResourceUrl;

    std::unordered_map<ObjectID, std::shared_ptr<Object>> m_objects;    
    std::shared_ptr<Object> m_sceneObject;
    std::shared_ptr<ResourceType::Level> m_levelResource;
};

}