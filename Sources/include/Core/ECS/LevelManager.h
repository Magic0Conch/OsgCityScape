#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include "ObjectIDAllocator.h"

namespace CSEditor::ECS{

class GObject;
class ObjectInstanceRes;

using LevelObjectsMap = std::unordered_map<GObjectID, std::shared_ptr<GObject>>;

class LevelManager
{
public:

    virtual ~LevelManager();

    bool load(const std::string& level_res_url);
    void unload();

    bool save();

    void tick(float delta_time);

    const std::string& getLevelResUrl() const;

    const LevelObjectsMap& getAllGObjects() const;

    std::weak_ptr<GObject>   getGObjectByID(GObjectID go_id) const;
    

    GObjectID createObject(const ObjectInstanceRes& object_instance_res);
    void deleteGObjectByID(GObjectID go_id);

protected:
    void clear();

    bool m_is_loaded {false};
    std::string m_level_res_url;

    LevelObjectsMap m_gobjects;
};

}