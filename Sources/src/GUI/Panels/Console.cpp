#include "GUI/Panels/Console.h"

using namespace CSEditor::GUI;

void Console::drawImpl(){
    const auto main_viewport = ImGui::GetMainViewport();
    ImGuiWindowFlags window_flags = 0;
    ImGui::SetNextWindowPos(ImVec2(main_viewport->GetWorkCenter()),ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(550,680),ImGuiCond_FirstUseEver);
    bool p_open = true;
    if(ImGui::Begin("Console",&p_open,window_flags)){
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