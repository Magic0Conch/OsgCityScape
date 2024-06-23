#include "Editor/Core/RuntimeContext.h"
#include "Core/ECS/WorldManager.h"
#include "Editor/Core/EditorInputManager.h"
#include "Resources/ResourceManagement/AssetManager.h"
#include "Resources/ResourceManagement/ConfigManager.h"
#include "Editor/Core/EditorInputManager.h"
#include "Render/RenderSystem.h"
#include "Windowing/Settings/WindowSettings.h"
#include "Windowing/Window.h"
#include "osg/ref_ptr"
#include <iostream>
#include <memory>
#include <filesystem>
namespace CSEditor::Core{

RuntimeContext g_runtimeContext;
RuntimeContext::RuntimeContext(){
    viewer = new osgViewer::Viewer();
    // osgViewer::GraphicsWindow
    //cam->setGraphicsContext(gc.get());

    //setup window
    char buffer[MAX_PATH];
    GetModuleFileNameA(nullptr, buffer, MAX_PATH);
    auto rootPath = std::filesystem::path(buffer).parent_path().parent_path().parent_path();
    std::cout<<rootPath<<std::endl; 
    auto configPath =  rootPath / "resources"/"configs"/"global_config.ini";
    configManager = std::make_unique<Resources::ConfigManager>(configPath);
    
    worldManager = std::make_unique<ECS::WorldManager>();

    Settings::WindowSettings windowSettings;
    windowSystem = std::make_unique<Windowing::WindowSystem>(windowSettings);
    
    renderSystem = std::make_unique<Render::RenderSystem>();
    eventManager = std::make_unique<Core::EventManager>();
    editorInputManager = std::make_unique<Core::EditorInputManager>(CSEditor::Core::g_runtimeContext.viewer->getCamera());

    assetManager = std::make_unique<Resources::AssetManager>();
    uiManager = new GUI::UIManager;
    // GUI::Hierachy hie;
    
}

RuntimeContext::~RuntimeContext(){    

}

}