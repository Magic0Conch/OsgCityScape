#include "Windowing/Window.h"
#include "Windowing/Settings/WindowSettings.h"
#include "spdlog/spdlog.h"
#include "Editor/Core/RuntimeContext.h"
#include <osg/Camera>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgViewer/Viewer>
using namespace CSEditor::Windowing;

// extern CSEditor::Core::RuntimeContext g_runtimeContext;

WindowSystem::WindowSystem(const Settings::WindowSettings& windowSettings):
    m_title(windowSettings.title),
    m_size(windowSettings.width,windowSettings.height),
    m_fullscreen(windowSettings.fullscreen),    
    m_decorated(windowSettings.decorated),
    m_samples(windowSettings.samples),
    m_position(0,0)
{
    createWindow(windowSettings);
}
WindowSystem::~WindowSystem(){
    
}
void WindowSystem::setSize(uint16_t width,uint16_t height){
    m_size = {width,height};
    m_viewport->setViewport(m_position.first,m_position.second, width, height);
}
void WindowSystem::setPosition(uint16_t x,uint16_t y){
    m_position = {x,y};
    m_viewport->setViewport(x, y, m_size.first,m_size.second);
}
void WindowSystem::show() const{

}
void WindowSystem::setTitle(const std::string& p_title){
    m_title = p_title;
    m_graphicsWindow->setWindowName(m_title);
}
std::string WindowSystem::getTitle() const{
    return m_title;
}

void WindowSystem::createWindow(const Settings::WindowSettings& windowSettings){     
    //monitor
    osg::GraphicsContext::getWindowingSystemInterface()->getScreenSettings(0, m_screenSettings);
    int x = m_screenSettings.width/2 - windowSettings.width/2;
    int y = m_screenSettings.height/2 - windowSettings.height/2;
    //window   
    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits();
    traits->windowName = windowSettings.title;
    traits->width = windowSettings.width; traits->height = windowSettings.height;    
    traits->windowDecoration = windowSettings.decorated;
    traits->samples = windowSettings.samples;    
    traits->doubleBuffer = true;
    traits->glContextProfileMask = 0x1;// 0x1;// 
    traits->x = x,traits->y = y;
    
    osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());    
    gc->getState()->resetVertexAttributeAlias(false);
    gc->getState()->setCheckForGLErrors(osg::State::CheckForGLErrors::ONCE_PER_ATTRIBUTE);
    
    if(m_fullscreen){
        Core::g_runtimeContext.viewer->setUpViewOnSingleScreen(0);
    }

    auto mainCamera = CSEditor::Core::g_runtimeContext.viewer->getCamera();
    mainCamera->setViewport(new osg::Viewport( m_position.first, m_position.second, windowSettings.width, windowSettings.height));
    m_viewport = mainCamera->getViewport();
    mainCamera->setGraphicsContext(gc);
    mainCamera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
    m_screenTexture = new Resources::RenderTexture(windowSettings.width,windowSettings.height);
    mainCamera->attach( osg::Camera::COLOR_BUFFER, m_screenTexture.get());
    m_screenTexture->getTextureTarget();
    osgViewer::Viewer::Windows windows;
    CSEditor::Core::g_runtimeContext.viewer->getWindows(windows);
    m_graphicsWindow = windows.front();
}

void WindowSystem::setGraphicsWindow(osg::ref_ptr<osgViewer::GraphicsWindow> graphicsWindow){
    m_graphicsWindow = graphicsWindow;
}
void WindowSystem::setViewport(osg::ref_ptr<osg::Viewport> viewport){
    m_viewport = viewport;
}

osg::ref_ptr<CSEditor::Resources::RenderTexture> WindowSystem::getScreenTexture(){
    return m_screenTexture;
}