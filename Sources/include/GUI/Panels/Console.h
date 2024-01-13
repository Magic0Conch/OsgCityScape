#pragma once
#include <imgui.h>
#include "GUI/Core/Panel.h"

namespace CSEditor::GUI {
class Console : public Panel{
public:
    void addLog(const char* fmt, ...) IM_FMTARGS(2);
    Console();
protected:
    virtual void drawImpl() override;
private:
    void Clear();

    ImGuiTextBuffer Buf;
    ImGuiTextFilter Filter;
    ImVector<int> LineOffsets; 
    bool AutoScroll;
};
}