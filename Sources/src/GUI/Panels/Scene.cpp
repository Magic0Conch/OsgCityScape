#include "GUI/Panels/Scene.h"
#include "Editor/Core/RuntimeContext.h"
#include "Core/Event.h"
#include "imgui.h"
#include "Windowing/Window.h"
#include <memory>
#include <utility>
#include "Editor/Core/EditorInputManager.h"
#include "osg/Vec2f"

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
        ImVec2 leftCorner = ImGui::GetCursorScreenPos();
        ImVec2 mousePos = ImGui::GetMousePos();
        osg::Vec2f scenePos = osg::Vec2f((mousePos.x - leftCorner.x)*1.0/width,(height - mousePos.y + leftCorner.y)*1.0/height);
        if(scenePos.x() < 0 || scenePos.y() < 0 || scenePos.x() > 1.0 || scenePos.y() > 1.0){
            scenePos = osg::Vec2f(-1,-1);
        }
        Core::g_runtimeContext.editorInputManager->setScenePosition(scenePos);
        ImGui::Image(imguiTextureID, ImVec2(width,height),uv0,uv1);
    }
    ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
    ImGui::PopItemWidth();
    ImGui::End();
}

std::pair<short, short> Scene::getScenePositon() const{
    return m_scenePosition;
}