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
        auto windowSize = ImGui::GetContentRegionAvail();
        float width = windowSize.x;
        float height = windowSize.y;
        ImVec2 uv0(0, 1);
        ImVec2 uv1(1, 0);
        ImGui::Image(imguiTextureID, ImVec2(width,height),uv0,uv1);
    }
    ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
    ImGui::PopItemWidth();
    ImGui::End();
}
