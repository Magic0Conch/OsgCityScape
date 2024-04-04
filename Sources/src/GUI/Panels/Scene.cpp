#include "GUI/Panels/Scene.h"
#include "Editor/Core/RuntimeContext.h"
#include "Core/Event.h"
#include "imgui.h"
#include "Windowing/Window.h"
#include <memory>

using namespace CSEditor::GUI;

Scene::Scene(){

}

void Scene::drawImpl(){
    const auto main_viewport = ImGui::GetMainViewport();
    ImGuiWindowFlags window_flags = 0;
    // ImGui::SetNextWindowPos(ImVec2(main_viewport->GetWorkCenter()),ImGuiCond_FirstUseEver);
    // ImGui::SetNextWindowSize(ImVec2(550,680),ImGuiCond_FirstUseEver);
    window_flags |= ImGuiWindowFlags_NoScrollbar;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    bool p_open = true;
    ImTextureID imguiTextureID = 0; 
    auto textureObject = Core::g_runtimeContext.windowSystem->getScreenTexture()->getTextureObject(0);
    if(textureObject){
        const auto textureId =  textureObject->id();
        imguiTextureID = (ImTextureID)(intptr_t)textureId;        
    }
    if(ImGui::Begin("Scene",&p_open,window_flags)){
        const auto& windowSize = ImGui::GetWindowSize();
        // onScenePanelSizeChanged->invoke(windowSize.x,windowSize.y);
        auto width = Core::g_runtimeContext.windowSystem->getScreenTexture()->getTextureWidth();
        auto height = Core::g_runtimeContext.windowSystem->getScreenTexture()->getTextureHeight();
        ImGui::Image(imguiTextureID, ImVec2(width,height));
    }
    ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
    ImGui::PopItemWidth();
    ImGui::End();
}
