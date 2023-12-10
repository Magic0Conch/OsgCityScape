#include "Editor/Core/RuntimeContext.h"
#include "Windowing/Settings/WindowSettings.h"
#include "Windowing/Window.h"
#include "osg/ref_ptr"
#include <memory>
namespace CSEditor::Core{

RuntimeContext g_runtimeContext;
RuntimeContext::RuntimeContext(){
    viewer = new osgViewer::Viewer();
    // osgViewer::GraphicsWindow
    //cam->setGraphicsContext(gc.get());

    //setup window
    Settings::WindowSettings windowSettings;
    window = std::make_unique<Windowing::Window>(windowSettings);
    
}

RuntimeContext::~RuntimeContext(){    

}

}