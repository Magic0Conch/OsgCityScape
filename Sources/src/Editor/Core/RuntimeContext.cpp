#include "Editor/Core/RuntimeContext.h"
#include "Windowing/Settings/WindowSettings.h"
#include "Windowing/Window.h"
#include <memory>
using namespace CSEditor::Core;


RuntimeContext::RuntimeContext(){
    m_viewer = new osgViewer::Viewer();
    
    Settings::WindowSettings windowSettings;
    m_window = std::make_unique<Windowing::Window>(windowSettings);
    
    osg::ref_ptr<osgViewer::GraphicsWindow> m_graphicsWindow;
    osg::ref_ptr<osg::Viewport> m_viewport;

}

RuntimeContext::~RuntimeContext(){
    

}

