#pragma once
#include "Resources/ResourceManagement/AssetManager.h"
#include <osgViewer/Viewer>
#include <osg/GraphicsContext>
#include <osg/ref_ptr>
#include <memory>
namespace CSEditor{
    namespace  Windowing {
        class WindowSystem;
    }
    namespace Resources {
        class ConfigManager;
    }
    namespace ECS {
        class WorldManager;
    }
}

namespace CSEditor::Core {
class RuntimeContext{
public:
    RuntimeContext();
    ~RuntimeContext();

    osg::ref_ptr<osgViewer::Viewer> viewer;
    std::unique_ptr<CSEditor::Windowing::WindowSystem> windowSystem;
    std::unique_ptr<CSEditor::ECS::WorldManager> worldManager;
    std::unique_ptr<Resources::ConfigManager> configManager;
    std::unique_ptr<Resources::AssetManager> assetManager;
};

extern RuntimeContext g_runtimeContext;
}