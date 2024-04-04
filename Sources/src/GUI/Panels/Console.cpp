#include "GUI/Panels/Console.h"
#include "Editor/Core/RuntimeContext.h"

using namespace CSEditor::GUI;

void Console::addLog(const char* fmt, ...)
{
    int old_size = Buf.size();
    va_list args;
    va_start(args, fmt);
    Buf.appendfv(fmt, args);
    va_end(args);
    for (int new_size = Buf.size(); old_size < new_size; old_size++)
        if (Buf[old_size] == '\n')
            LineOffsets.push_back(old_size + 1);
}

void Console::Clear()
{
    Buf.clear();
    LineOffsets.clear();
    LineOffsets.push_back(0);
}

Console::Console(){
    AutoScroll = true;
    Clear();
}

void Console::drawImpl(){
    bool p_open = true;
    // ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("Console", &p_open);
    if (ImGui::SmallButton("[Debug] Log a info"))
    {
        Core::g_runtimeContext.logSystem->info("info!");
        Core::g_runtimeContext.logSystem->error("error!");
        Core::g_runtimeContext.logSystem->info("info!");

    }
    ImGui::End();
    if (!ImGui::Begin("Console", &p_open))
    {
        ImGui::End();
        return;
    }

    if (ImGui::BeginPopup("Options"))
    {
        ImGui::Checkbox("Auto-scroll", &AutoScroll);
        ImGui::EndPopup();
    }

    if (ImGui::Button("Options"))
        ImGui::OpenPopup("Options");
    ImGui::SameLine();
    bool clear = ImGui::Button("Clear");
    ImGui::SameLine();
    bool copy = ImGui::Button("Copy");
    ImGui::SameLine();
    Filter.Draw("Filter", -100.0f);

    ImGui::Separator();
    ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

    if (clear)
        Clear();
    if (copy)
        ImGui::LogToClipboard();

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    const char* buf = Buf.begin();
    const char* buf_end = Buf.end();
    if (Filter.IsActive())
    {
        for (int line_no = 0; line_no < LineOffsets.Size; line_no++)
        {
            const char* line_start = buf + LineOffsets[line_no];
            const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
            if (Filter.PassFilter(line_start, line_end))
                ImGui::TextUnformatted(line_start, line_end);
        }
    }
    else
    {
        ImGuiListClipper clipper;
        clipper.Begin(LineOffsets.Size);
        while (clipper.Step())
        {
            for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
            {
                const char* line_start = buf + LineOffsets[line_no];
                const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                ImGui::TextUnformatted(line_start, line_end);
            }
        }
        clipper.End();
    }
    ImGui::PopStyleVar();

    if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);

    ImGui::EndChild();
    ImGui::End();
}