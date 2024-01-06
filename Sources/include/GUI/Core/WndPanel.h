#pragma once
#include <string>
#include "GUI/Core/Panel.h"
#include "imgui.h"

namespace CSEditor::GUI{

class WndPanel:public Panel {
public:
    WndPanel();
    void drawImpl() override;
    void updateWndStates();

    std::pair<int,int> getImguiSize();
    std::pair<int,int> getImguiPos();
    std::pair<int,int> getCanvasSize();
    std::pair<int,int> getCanvasPos();
    std::string name_;
    bool resizable_=true;
    bool closable_=true;
    bool movable_=true;
    bool dockable_=true;
    bool scrollable_=true;
    bool hori_scrollable_=false;
    bool isoverlay_ = false;
    std::pair<int,int> size_={65535,65535};
    std::pair<int,int> pos_;
    std::pair<int,int> canvas_size_={65535,65535};
    std::pair<int,int> canvas_pos_;
    ImVec2 window_padding_={15,15};


    bool opened_=true;
    bool hovered_=false;
    bool focused_=false;

    
};

}