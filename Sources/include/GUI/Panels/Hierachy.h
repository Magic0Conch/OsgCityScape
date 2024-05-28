#pragma once
#include <imgui.h>
#include <memory>
#include <string>
#include <vector>
#include "Core/ECS/Level.h"
#include "Core/ECS/ObjectIDAllocator.h"
#include "Editor/Core/RuntimeContext.h"
#include "GUI/Core/Panel.h"
#include "Core/ECS/Object.h"
#include "GUI/Event/UIEventManager.h"

namespace CSEditor::GUI {
class Hierachy : public Panel{
public:
    Hierachy();
    void setNodeClicked(int nodeClicked);;
protected:
    void drawHierachyNodeRecursively(ECS::ObjectID objectID);
    virtual void drawImpl() override;
private:
    ImGuiTextFilter m_filter;
    std::shared_ptr<ECS::Level> m_level;
    std::unordered_map<ECS::ObjectID, std::shared_ptr<ECS::Object>>& m_objectsMap;
    int m_nodeClicked = -1;
    std::shared_ptr<Core::Event<int>> onSelectedObjectChanged;
};
}
