#include "GUI/Panels/Project.h"

using namespace CSEditor::GUI;

void Project::drawImpl(){
    const auto main_viewport = ImGui::GetMainViewport();
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    ImGui::SetNextWindowPos(ImVec2(main_viewport->GetWorkCenter()),ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(550,680),ImGuiCond_FirstUseEver);
    bool p_open = true;
    if(ImGui::Begin("Project",&p_open,window_flags)){
        if (ImGui::TreeNode("Transform Properties")){

            ImGui::TreePop();
        }

        if(ImGui::TreeNode("Wave Properties")){

            ImGui::TreePop();
        }
        if(ImGui::TreeNode("Blur Properties")){
            // ImGui::SliderInt("blurIterations", &blurIterations,0,32);
            ImGui::TreePop();
        }
    }
    ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
    ImGui::PopItemWidth();
    ImGui::End();
    ImGui::ShowDemoWindow();
}