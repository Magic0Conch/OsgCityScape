#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include "Core/ECS/Object.h"
#include "ObjectIDAllocator.h"
#include "Core/ECS/Level.h"
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

    // Level();
    virtual ~Level();

    bool load(const std::string& levelResourceUrl);
    void unload();

    bool save();

    void tick(float delta_time);

    const std::string& getLevelResUrl() const;

    void buildSceneGraph();

    ObjectID createObject(const ResourceType::ObjectInstance& objectInstance);

    // const LevelObjectsMap& getAllGObjects() const;

    // std::weak_ptr<GObject>   getGObjectByID(GObjectID go_id) const;
    

    // GObjectID createObject(const ObjectInstanceRes& object_instance_res);
    // void deleteGObjectByID(GObjectID go_id);

protected:
    void clear();

    bool m_isLoaded {false};
    std::string m_levelResourceUrl;

    std::unordered_map<ObjectID, std::shared_ptr<Object>> m_objects;    
    std::shared_ptr<Object> m_sceneObject;
};

}