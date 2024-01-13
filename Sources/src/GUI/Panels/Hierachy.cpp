#include "GUI/Panels/Hierachy.h"

using namespace CSEditor::GUI;

void Hierachy::drawImpl(){
    const auto main_viewport = ImGui::GetMainViewport();
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    ImGui::SetNextWindowPos(ImVec2(main_viewport->GetWorkCenter()),ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(550,680),ImGuiCond_FirstUseEver);
    bool p_open = true;
    if(ImGui::Begin("Hierachy",&p_open,window_flags)){
        static int node_clicked = -1;
        for (int i = 0; i < 6; i++)
        {
            ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
            if (i == node_clicked)
                node_flags |= ImGuiTreeNodeFlags_Selected;
            bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Node %d", i);
            if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                node_clicked = i;
            if (node_open)
            {
                ImGui::BulletText("Blah blah\nBlah Blah");
                ImGui::TreePop();
            }
        }
        // if (node_clicked != -1)
        // {
        //     // Update selection state
        //     // (process outside of tree loop to avoid visual inconsistencies during the clicking frame)
        //     // if (ImGui::GetIO().KeyCtrl)
        //     //     selection_mask ^= (1 << node_clicked);          // CTRL+click to toggle
        //     // else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, may want to preserve selection when clicking on item that is part of the selection
        //     //     selection_mask = (1 << node_clicked);           // Click to single-select
        // }
    }
    ImGui::End();
    ImGui::ShowDemoWindow();
}
