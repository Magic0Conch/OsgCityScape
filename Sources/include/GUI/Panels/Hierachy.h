#pragma once
#include <imgui.h>
#include <string>
#include <vector>
#include "Core/ECS/ObjectIDAllocator.h"
#include "Editor/Core/RuntimeContext.h"
#include "GUI/Core/Panel.h"
#include "Core/ECS/Object.h"
#include "GUI/Event/UIEventManager.h"

namespace CSEditor::GUI {
class Hierachy : public Panel{
public:
    Hierachy();
protected:
    void drawHierachyNodeRecursively(ECS::ObjectID objectID);
    virtual void drawImpl() override;
private:
    ImGuiTextFilter m_filter;
    std::unordered_map<ECS::ObjectID, std::shared_ptr<ECS::Object>>& m_objectsMap;
    int m_nodeClicked = -1;

};
}