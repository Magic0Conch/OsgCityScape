#include "GUI/Panels/Inspector.h"
#include "imgui.h"

using namespace CSEditor::GUI;

void Inspector::drawImpl(){
    const auto main_viewport = ImGui::GetMainViewport();
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    ImGui::SetNextWindowPos(ImVec2(main_viewport->GetWorkCenter()),ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(550,680),ImGuiCond_FirstUseEver);
    bool p_open = true;
    if(ImGui::Begin("Inspector",&p_open,window_flags)){
        ImGui::SetNextItemWidth(ImGui::GetWindowWidth());
        char objectName[128] = "Fence";
        static bool check = true;
        ImGui::AlignTextToFramePadding();
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 0));
        ImGui::Checkbox("##1", &check);
        ImGui::PopStyleVar(1);
        ImVec2 cursorPos = ImGui::GetCursorPos();
        ImGui::SameLine();
        ImGui::SetCursorPosY(cursorPos.y/2);

        ImGui::InputText("##2", objectName, IM_ARRAYSIZE(objectName));
        ImGui::Separator();

        if (ImGui::CollapsingHeader("Transform")){

        }
        if (ImGui::CollapsingHeader("Mesh")){

        }
    }
    ImGui::End();
}