#include "Editor/Core/RuntimeContext.h"
#include "Core/ECS/WorldManager.h"
#include "Editor/Core/EditorInputManager.h"
#include "Resources/ResourceManagement/AssetManager.h"
#include "Resources/ResourceManagement/ConfigManager.h"
#include "Editor/Core/EditorInputManager.h"
#include "Render/RenderSystem.h"
#include "Windowing/Settings/WindowSettings.h"
#include "GUI/Panels/Hierachy.h"
#include "Windowing/Window.h"
#include "osg/ref_ptr"
#include <memory>
#include <filesystem>
namespace CSEditor::Core{

RuntimeContext g_runtimeContext;
RuntimeContext::RuntimeContext(){
    viewer = new osgViewer::Viewer();
    // osgViewer::GraphicsWindow
    //cam->setGraphicsContext(gc.get());

    //setup window
    auto rootPath = std::filesystem::current_path().parent_path().parent_path();
    auto configPath =  rootPath / "resources"/"configs"/"global_config.ini";
    configManager = std::make_unique<Resources::ConfigManager>(configPath);
    
    eventManager = std::make_unique<Core::EventManager>();
    worldManager = std::make_unique<ECS::WorldManager>();
    renderSystem = std::make_unique<Render::RenderSystem>();

    Settings::WindowSettings windowSettings;
    windowSystem = std::make_unique<Windowing::WindowSystem>(windowSettings);
    editorInputManager = std::make_unique<Core::EditorInputManager>(CSEditor::Core::g_runtimeContext.viewer->getCamera());

    assetManager = std::make_unique<Resources::AssetManager>();
    uiManager = new GUI::UIManager;
    // GUI::Hierachy hie;
    
}

RuntimeContext::~RuntimeContext(){    

}

}