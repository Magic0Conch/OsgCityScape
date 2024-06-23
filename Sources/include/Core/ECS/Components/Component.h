#pragma once
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
// #include "Core/ECS/Object.h"
#include "Core/Helpers/ISerializer.h"
#include "spdlog/spdlog.h"
// #include "Core/ECS/Object.h"
namespace CSEditor::ECS{
class Object;
class Component:public Helpers::ISerializable{
protected:
    std::weak_ptr<Object> m_parentObject;
    // bool m_is_dirty {false};
    // bool m_is_scale_dirty {false};
    std::string m_typename;
public: 
    Component() = default;
    virtual ~Component() {}

    virtual void serialize(Json& jsonObject);
    virtual void deserialize(Json& jsonObject);
    // Instantiating the component after definition loaded
    virtual void loadResource(std::shared_ptr<Object> parentObject);

    virtual void tick(float delta_time) {};

    virtual void onComponentAdded(std::shared_ptr<Object> parentObject);
    virtual void drawImGui();
    std::string returnTypeName();
    void setTypeName(std::string typeName);
    // bool isDirty() const { return m_is_dirty; }

    // void setDirtyFlag(bool is_dirty) { m_is_dirty = is_dirty; }

    bool m_tickInEditorMode {false};
};
}