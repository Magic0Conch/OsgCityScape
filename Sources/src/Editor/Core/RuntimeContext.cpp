#include "Editor/Core/RuntimeContext.h"
#include "Core/ECS/WorldManager.h"
#include "Resources/ResourceManagement/AssetManager.h"
#include "Resources/ResourceManagement/ConfigManager.h"
#include "Windowing/Settings/WindowSettings.h"
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
    
    worldManager = std::make_unique<ECS::WorldManager>();

    Settings::WindowSettings windowSettings;
    windowSystem = std::make_unique<Windowing::WindowSystem>(windowSettings);

    assetManager = std::make_unique<Resources::AssetManager>();
    uiManager = std::make_unique<GUI::UIManager>();
    

}

RuntimeContext::~RuntimeContext(){    

}

}