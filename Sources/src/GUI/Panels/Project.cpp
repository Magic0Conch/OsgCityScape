#include "GUI/Panels/Project.h"

using namespace CSEditor::GUI;

Project::Project():
m_folders{"Assets","Resources","Textures","Models"}{

}

void Project::drawImpl(){
    const auto main_viewport = ImGui::GetMainViewport();
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    ImGui::SetNextWindowPos(ImVec2(main_viewport->GetWorkCenter()),ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(550,680),ImGuiCond_FirstUseEver);
    bool p_open = true;
    if(ImGui::Begin("Project",&p_open,window_flags)){
        // Left
        static int selected = 0;
        {
            ImGui::BeginChild("left panel", ImVec2(150, 0), true);
            for(int i = 0;i<m_folders.size();i++){
                const auto & folder = m_folders[i];
                ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
                if (i == selected)
                    node_flags |= ImGuiTreeNodeFlags_Selected;
                bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, &folder[0]);
                if (node_open)
                {
                    ImGui::BulletText("Blah blah\nBlah Blah");
                    ImGui::TreePop();
                }
                if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                    selected = i;
            }
            ImGui::EndChild();
        }
        ImGui::SameLine();

        // Right
        {
            ImGui::BeginGroup();
            ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
            ImGui::Text("MyObject: %d", selected);
            ImGui::Separator();
            ImGui::EndChild();
            ImGui::EndGroup();
        }
    }
    ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
    ImGui::PopItemWidth();
    ImGui::End();
    ImGui::ShowDemoWindow();
}