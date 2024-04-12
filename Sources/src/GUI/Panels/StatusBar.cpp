#include "GUI/Panels/StatusBar.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "Editor/Core/RuntimeContext.h"
#include "Windowing/Window.h"
#include "Core/Math/MatrixHelper.h"
#include "osg/Vec3f"

using namespace CSEditor::GUI;

StatusBar::StatusBar(){

}



void StatusBar::drawImpl(){
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
    bool p_open = true;
    ImGuiViewportP* viewport = (ImGuiViewportP*)(void*)ImGui::GetMainViewport();
    float height = ImGui::GetFrameHeight();

    auto camera = Core::g_runtimeContext.windowSystem->getMainCamera();
    osg::Vec3d eye, center, up;
    camera->getViewMatrixAsLookAt(eye, center, up);
    osg::Vec3f dir = (center - eye);
    dir.normalize();


    std::stringstream ss;
    ss << "Camera Position: X=" << eye.x() << ", Y=" << eye.y() << ", Z=" << eye.z();

    if (ImGui::BeginViewportSideBar("##MainStatusBar",viewport, ImGuiDir_Down, height, window_flags)) {
        if (ImGui::BeginMenuBar()) {
            ImGui::Text("%s", ss.str().c_str());
            ImGui::EndMenuBar();
        }
    ImGui::End();
}

}