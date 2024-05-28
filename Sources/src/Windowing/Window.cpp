#include "Windowing/Window.h"
#include "Windowing/Settings/WindowSettings.h"
#include "osg/Group"
#include "spdlog/spdlog.h"
#include "Editor/Core/RuntimeContext.h"
#include <osg/Camera>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgViewer/Viewer>
#include "Render/LowRender/RenderColorToTexture.h"

using namespace CSEditor::Windowing;

// extern CSEditor::Core::RuntimeContext g_runtimeContext;

WindowSystem::WindowSystem(Settings::WindowSettings& windowSettings):
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
    m_screenTexture->setTextureSize(width, height);

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

void WindowSystem::createWindow(Settings::WindowSettings& windowSettings){     
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
    
    m_graphicsContext = osg::GraphicsContext::createGraphicsContext(traits.get());    
    m_graphicsContext->getState()->resetVertexAttributeAlias(false);
    m_graphicsContext->getState()->setCheckForGLErrors(osg::State::CheckForGLErrors::ONCE_PER_ATTRIBUTE);
    
    if(m_fullscreen){
        Core::g_runtimeContext.viewer->setUpViewOnSingleScreen(0);
    }

    m_mainCamera = CSEditor::Core::g_runtimeContext.viewer->getCamera();
    m_mainCamera->setViewport(new osg::Viewport( m_position.first, m_position.second, windowSettings.width, windowSettings.height));
    m_viewport = m_mainCamera->getViewport();
    m_mainCamera->setGraphicsContext(m_graphicsContext);
    // m_mainCamera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
    m_mainCamera->setCullMask(0x1);
    m_screenTexture = new Resources::RenderTexture(windowSettings.width,windowSettings.height);
    // mainCamera->attach( osg::Camera::COLOR_BUFFER, m_screenTexture.get());
    // mainCamera->setCullMask(0x0);
    osgViewer::Viewer::Windows windows;
    CSEditor::Core::g_runtimeContext.viewer->getWindows(windows);
    m_graphicsWindow = windows.front();
    // osg::setNotifyLevel(osg::FATAL);

    // Render::RenderColorToTexture *rtt = new Render::RenderColorToTexture();
    // rtt->setGraphicsContext(m_graphicsContext);
    // rtt->setViewport(new osg::Viewport(0,0, traits->width, traits->height));
    // Core::g_runtimeContext.viewer->addSlave(rtt);
}

void WindowSystem::updateViewportSize(int width,int height){
    setSize(width, height);
}

void WindowSystem::setGraphicsWindow(osg::ref_ptr<osgViewer::GraphicsWindow> graphicsWindow){
    m_graphicsWindow = graphicsWindow;
}
void WindowSystem::setViewport(osg::ref_ptr<osg::Viewport> viewport){
    m_viewport = viewport;
}

osg::ref_ptr<osg::Texture2D> WindowSystem::getScreenTexture(){
    return m_screenTexture;
}

osg::ref_ptr<osg::GraphicsContext> WindowSystem::getGraphicsContext() const{
    return m_graphicsContext;
}

void WindowSystem::setScreenTexture(osg::ref_ptr<osg::Texture2D> screenTexture){
    m_screenTexture = screenTexture;
}

osg::ref_ptr<osg::Camera> WindowSystem::getMainCamera() const{
    return m_mainCamera;
}

osg::ref_ptr<osg::Viewport> WindowSystem::getViewport() const{
    return m_viewport;
}

std::pair<uint16_t, uint16_t> WindowSystem::getSize() const{
    return m_size;
}
// osg::ref_ptr<osg::Group> WindowSystem::getRootNode(){
//     return m_rootNode;
// }