#pragma once
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
// #include "Core/ECS/Object.h"
#include "Core/Helpers/ISerializer.h"
// #include "Core/ECS/Object.h"
namespace CSEditor::ECS{
class Object;
class Component:public Helpers::ISerializable{
protected:
    std::weak_ptr<Object> m_parentObject;
    // bool m_is_dirty {false};
    // bool m_is_scale_dirty {false};

public: 
    Component() = default;
    virtual ~Component() {}

    virtual void serialize(Json& jsonObject);
    virtual void deserialize(Json& jsonObject);
    // Instantiating the component after definition loaded
    virtual void postLoadResource(std::weak_ptr<Object> parentObject);

    virtual void tick(float delta_time) {};

    // bool isDirty() const { return m_is_dirty; }

    // void setDirtyFlag(bool is_dirty) { m_is_dirty = is_dirty; }

    bool m_tickInEditorMode {false};
};
}