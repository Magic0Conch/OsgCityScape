#include "Core/Helpers/LogSystem.h"
#include "Editor/Core/RuntimeContext.h"

using namespace CSEditor::Helpers;

LogSystem::LogSystem():m_console(Core::g_runtimeContext.uiManager->getPanel<GUI::Console>("Console")),m_counter(0){
    
}

void LogSystem::info(std::string&& msg){
    // const char* categories[3] = { "info", "warn", "error" };
    m_console.addLog("[%05d] [%s]  %.1f,'%s'\n",
    ImGui::GetFrameCount(), "info", ImGui::GetTime(), msg.c_str());
    ++m_counter;
}


void LogSystem::error(std::string&& msg){
    m_console.addLog("[%05d] [%s]  %.1f,'%s'\n",
    ImGui::GetFrameCount(), "error", ImGui::GetTime(), msg.c_str());
    ++m_counter;
}

void LogSystem::warn(std::string&& msg){
    m_console.addLog("[%05d] [%s]  %.1f,'%s'\n",
    ImGui::GetFrameCount(), "warn", ImGui::GetTime(), msg.c_str());
    ++m_counter;
}