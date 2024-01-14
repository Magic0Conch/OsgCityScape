#include "GUI/Panels/Inspector.h"
#include "Core/ECS/ObjectIDAllocator.h"
#include "Core/Event.h"
#include "Editor/Core/RuntimeContext.h"
#include "Core/ECS/WorldManager.h"
#include "imgui.h"
#include <functional>
#include <string>
#include "GUI/Event/UIEventManager.h"

using namespace CSEditor::GUI;

Inspector::Inspector():
m_objectsMap(Core::g_runtimeContext.worldManager->getCurrentActiveLevel()->getSceneObjectsMap()),
m_position{0.0,0.0,0.0},
m_rotation{1.0,0.0,0.0,0.0},
m_scale{1.0,1.0,1.0}{
    Core::Event<ECS::ObjectID>::Callback onClickedIndexChangedCallback = std::bind(&Inspector::onClickedIndexChanged,this,std::placeholders::_1);
    UIEveneManager::getInstance().selectHierachyEvent+=onClickedIndexChangedCallback;
}

void Inspector::onClickedIndexChanged(ECS::ObjectID clickedID){
    Core::g_runtimeContext.logSystem->info(std::to_string(clickedID) + "info show!");
    m_selectedObjectID = clickedID;
}

void Inspector::drawImpl(){
    const auto main_viewport = ImGui::GetMainViewport();
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    ImGui::SetNextWindowPos(ImVec2(main_viewport->GetWorkCenter()),ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(550,680),ImGuiCond_FirstUseEver);
    bool p_open = true;
    if(ImGui::Begin("Inspector",&p_open,window_flags)){
        if(m_selectedObjectID==-1||m_selectedObjectID==0){
            ImGui::End();
            return;
        }
        auto& curSceneObject = *m_objectsMap[m_selectedObjectID];
        ImGui::SetNextItemWidth(ImGui::GetWindowWidth());
        auto objectNameStr =  curSceneObject.getName();
        auto objectNameCStr = const_cast<char*>(objectNameStr.c_str());
        std::string meshPath(128,'\0');
        static bool check = true;
        ImGui::AlignTextToFramePadding();
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 0));
        ImGui::Checkbox("##1", &check);
        ImGui::PopStyleVar(1);
        ImVec2 cursorPos = ImGui::GetCursorPos();
        ImGui::SameLine();
        ImGui::SetCursorPosY(cursorPos.y/2);

        ImGui::InputText("##2", objectNameCStr, 128,0,NULL,NULL);
        ImGui::Separator();

        if (ImGui::CollapsingHeader("Transform")){
            ImGui::InputFloat3("Position", &m_position[0]);
            ImGui::InputFloat4("Rotation", &m_rotation[0]);
            ImGui::InputFloat3("Scale", &m_scale[0]);
        }
        if (ImGui::CollapsingHeader("Mesh")){            
            ImGui::InputText("Mesh Path", &meshPath[0], 128);
        }
    }
    ImGui::End();
}