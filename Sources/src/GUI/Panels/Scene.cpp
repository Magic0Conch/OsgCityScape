#include "GUI/Panels/Scene.h"
#include "Editor/Core/RuntimeContext.h"
#include "imgui.h"
#include "Windowing/Window.h"

using namespace CSEditor::GUI;

void Scene::drawImpl(){
    const auto main_viewport = ImGui::GetMainViewport();
    ImGuiWindowFlags window_flags = 0;
    // ImGui::SetNextWindowPos(ImVec2(main_viewport->GetWorkCenter()),ImGuiCond_FirstUseEver);
    // ImGui::SetNextWindowSize(ImVec2(550,680),ImGuiCond_FirstUseEver);
    window_flags |= ImGuiWindowFlags_NoScrollbar;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    bool p_open = true;
    if(ImGui::Begin("Scene",&p_open,window_flags)){
        ImGui::Image((ImTextureID)2, main_viewport->Size);
    }
    ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
    ImGui::PopItemWidth();
    ImGui::End();
}
