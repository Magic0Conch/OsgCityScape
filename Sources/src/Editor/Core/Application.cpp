#include "Editor/Core/Application.h"
#include "spdlog/spdlog.h"

using namespace CSEditor::Core;

Application::Application(){

}

Application::~Application(){

}

void Application::run(){
    spdlog::info("Engine start!");
    m_editor.run();
}