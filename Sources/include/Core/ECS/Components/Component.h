#pragma once
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "Core/Helpers/ISerializer.h"
namespace CSEditor::ECS{
class Component:public Helpers::ISerializable
{

protected:
    // std::weak_ptr<GObject> m_parent_object;
    // bool m_is_dirty {false};
    // bool m_is_scale_dirty {false};

public: 

    Component() = default;
    virtual ~Component() {}

    virtual void serialize(Json& jsonObject) override;
    virtual void deserialize(Json& jsonObject) override;
    // Instantiating the component after definition loaded
    // virtual void postLoadResource(std::weak_ptr<GObject> parent_object) { m_parent_object = parent_object; }

    virtual void tick(float delta_time) {};

    // bool isDirty() const { return m_is_dirty; }

    // void setDirtyFlag(bool is_dirty) { m_is_dirty = is_dirty; }

    bool m_tick_in_editor_mode {false};
};
}