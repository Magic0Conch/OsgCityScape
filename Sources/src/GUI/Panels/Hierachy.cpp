#include "GUI/Panels/Hierachy.h"
#include "Editor/Core/RuntimeContext.h"
#include "Core/ECS/WorldManager.h"
#include "Core/ECS/Level.h"
#include "imgui.h"

using namespace CSEditor::GUI;

Hierachy::Hierachy():m_level(Core::g_runtimeContext.worldManager->getCurrentActiveLevel()),m_objectsMap(m_level->getSceneObjectsMap()){
}

void Hierachy::drawHierachyNodeRecursively(ECS::ObjectID objectID){
    const auto& curObject = m_level->getSceneObjectById(objectID);
    ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick ;
    if (curObject->getID() == m_nodeClicked){
        nodeFlags |= ImGuiTreeNodeFlags_Selected;
    }
    if(curObject->isLeaf()){
        nodeFlags |= ImGuiTreeNodeFlags_Leaf;
    }
    bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)objectID, nodeFlags, "%s", &curObject->getName()[0]);
    if (ImGui::IsItemClicked()){
        m_nodeClicked = objectID;
        auto lastObjectID = m_level->getSelectedObjectID();
        m_level->setSelectedObjectID(objectID);
        UIEveneManager::getInstance().selectHierachyEvent.invoke(m_nodeClicked);
    }
    if(nodeOpen){
        for (const auto& id : curObject->getChildIndex()) {
            ImGui::SetCursorPosX((ImGui::GetCursorPosX() - ImGui::GetTreeNodeToLabelSpacing())*1.5);
            drawHierachyNodeRecursively(id);
        }
        ImGui::TreePop();
    }
}

void Hierachy::drawImpl(){
    const auto main_viewport = ImGui::GetMainViewport();
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    // ImGui::SetNextWindowPos(ImVec2(main_viewport->GetWorkCenter()),ImGuiCond_FirstUseEver);
    // ImGui::SetNextWindowSize(ImVec2(550,680),ImGuiCond_FirstUseEver);
    bool p_open = true;
    if(ImGui::Begin("Hierachy",&p_open,window_flags)){
        m_filter.Draw("search",-50);
        // auto& keys = m_objectsMap.
        if(m_filter.IsActive()){
            for (const auto&[objectID,object]:m_objectsMap) {
                ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
                if(!m_filter.PassFilter(&object->getName()[0]))
                    continue;
                if (objectID == m_nodeClicked)
                    node_flags |= ImGuiTreeNodeFlags_Selected;
                bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)objectID, node_flags, "%s", &object->getName()[0]);
                if (node_open)
                {
                    ImGui::BulletText("Blah blah\nBlah Blah");
                    ImGui::TreePop();
                }
                if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()){
                    m_nodeClicked = objectID;                
                    Core::g_runtimeContext.worldManager->getCurrentActiveLevel();
                }
                
            }
        }
        else {
            drawHierachyNodeRecursively(0);
        }
    }
    ImGui::End();
    ImGui::ShowDemoWindow();
}
