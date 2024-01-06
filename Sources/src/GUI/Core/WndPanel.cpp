#include "GUI/Core/WndPanel.h"

using namespace CSEditor::GUI;

WndPanel::WndPanel(){}

std::pair<int,int> WndPanel::getImguiSize(){
    ImVec2 sz = ImGui::GetWindowSize();
    return{sz.x,sz.y};
}
std::pair<int,int> WndPanel::getImguiPos(){
    ImVec2 pos = ImGui::GetWindowPos();
    return{pos.x,pos.y};
}
std::pair<int,int> WndPanel::getCanvasPos(){
    ImVec2 pos = ImGui::GetCursorScreenPos(); 
    return{pos.x,pos.y};
}
std::pair<int,int> WndPanel::getCanvasSize(){
    int barheight=canvas_pos_.second-pos_.second;
    return{size_.first,size_.second-barheight};
}

void WndPanel::updateWndStates(){
    pos_=getImguiPos();
    canvas_pos_=getCanvasPos();
    size_=getImguiSize();
    canvas_size_=getCanvasSize();
}

void WndPanel::drawImpl(){
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, window_padding_);
    if (opened_){
        int windowFlags = ImGuiWindowFlags_None;
       
		if (!resizable_)				windowFlags |= ImGuiWindowFlags_NoResize;
		if (!movable_)					windowFlags |= ImGuiWindowFlags_NoMove;
		if (!dockable_)					windowFlags |= ImGuiWindowFlags_NoDocking;
        if (!scrollable_)               windowFlags |= ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar;
        if (hori_scrollable_)           windowFlags |= ImGuiWindowFlags_HorizontalScrollbar;

        if (isoverlay_) windowFlags=ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
        ImVec2 minszcons = {0,0};
        ImVec2 maxszcons = {10000,10000};
        ImGui::SetNextWindowSizeConstraints(minszcons,maxszcons);
        if (ImGui::Begin(name_.c_str(),&opened_,windowFlags)){
            hovered_ = ImGui::IsWindowHovered(0);
            focused_ = ImGui::IsWindowFocused();
            
            updateWndStates();
            
            // ImVec2 tmp = ImGui::GetCursorScreenPos(); 
            // ImVec2 tmp2 = ImGui::GetWindowPos();
           // std::cout << tmp2.x << " " <<tmp2.y<<" : ";
            //std::cout << tmp.x << " - " << tmp.y << "         \r";
            //ShowExampleAppSimpleOverlay(&opened_,ImVec2(pos_.first,pos_.second),ImVec2(size_.first,size_.second));
            drawUi();
            //ImGui::Image((void*)fbo_.tex_buf_id_, ImVec2(canvas_size_.first,canvas_size_.second), ImVec2(0.f, 1.f), ImVec2(1.f, 0.f));
        }
        ImGui::End();
    }
    ImGui::PopStyleVar();
}
