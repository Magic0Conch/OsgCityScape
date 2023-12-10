#include "Windowing/Window.h"
#include "Windowing/Settings/WindowSettings.h"
#include "osg/Viewport"
#include "spdlog/spdlog.h"
#include <osgViewer/Viewer>
#include "Editor/Core/RuntimeContext.h"
using namespace CSEditor::Windowing;

extern CSEditor::Core::RuntimeContext g_runtimeContext;

Window::Window(const Settings::WindowSettings& windowSettings):
    m_title(windowSettings.title),
    m_size(windowSettings.width,windowSettings.height),
    m_fullscreen(windowSettings.fullscreen),    
    m_decorated(windowSettings.decorated),
    m_samples(windowSettings.samples),
    m_position(0,0)
{
    osgViewer::Viewer::Windows windows;
    g_runtimeContext.viewer->getWindows(windows);
    m_graphicsWindow = windows.front();
    createWindow(windowSettings);
}
Window::~Window(){
    
}
void Window::setSize(uint16_t width,uint16_t height){
    m_size = {width,height};
    m_viewport->setViewport(m_position.first,m_position.second, width, height);
}
void Window::setPosition(uint16_t x,uint16_t y){
    m_position = {x,y};
    m_viewport->setViewport(x, y, m_size.first,m_size.second);
}
void Window::show() const{

}
void Window::setTitle(const std::string& p_title){
    m_title = p_title;
    m_graphicsWindow->setWindowName(m_title);
}
std::string Window::getTitle() const{
    return m_title;
}

void Window::createWindow(const Settings::WindowSettings& windowSettings){        
    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits();
    traits->windowName = windowSettings.title;
    traits->width = windowSettings.width; traits->height = windowSettings.height;    
    traits->windowDecoration = windowSettings.decorated;
    traits->samples = windowSettings.samples;    
    traits->doubleBuffer = true;
    traits->glContextProfileMask = 0x1;// 0x1;// 
    
    osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());    
    gc->getState()->resetVertexAttributeAlias(false);
    gc->getState()->setCheckForGLErrors(osg::State::CheckForGLErrors::ONCE_PER_ATTRIBUTE);
    
    auto mainCamera = g_runtimeContext.viewer->getCamera();
    mainCamera->setViewport(new osg::Viewport( m_position.first, m_position.second, windowSettings.width, windowSettings.height));
    m_viewport = mainCamera->getViewport();
    mainCamera->setGraphicsContext(gc);
    g_runtimeContext.viewer->realize();
}

void Window::setGraphicsWindow(osg::ref_ptr<osgViewer::GraphicsWindow> graphicsWindow){
    m_graphicsWindow = graphicsWindow;
}
void Window::setViewport(osg::ref_ptr<osg::Viewport> viewport){
    m_viewport = viewport;
}
