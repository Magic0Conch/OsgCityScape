#include "Windowing/Window.h"
#include "Windowing/Settings/WindowSettings.h"
#include "spdlog/spdlog.h"
#include <osgViewer/Viewer>

using namespace CSEditor::Windowing;

Window::Window(const Settings::WindowSettings& windowSettings):
    m_title(windowSettings.title),
    m_size(windowSettings.width,windowSettings.height),
    m_fullscreen(windowSettings.fullscreen),    
    m_decorated(windowSettings.decorated),
    m_samples(windowSettings.samples),
    m_position(0,0)
{
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
    
}

void Window::setGraphicsWindow(osg::ref_ptr<osgViewer::GraphicsWindow> graphicsWindow){
    m_graphicsWindow = graphicsWindow;
}
void Window::setViewport(osg::ref_ptr<osgViewer::GraphicsWindow> viewport){
    m_viewport = viewport;
}
