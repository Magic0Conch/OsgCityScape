#pragma once

#include "Settings/WindowSettings.h"
#include "osgViewer/View"
#include "osg/ref_ptr"
#include <memory>
#include <stdint.h>
#include <osg/GraphicsContext>
#include "Resources/RenderTexture.h"

namespace CSEditor::Windowing {

class WindowSystem{
public:
    WindowSystem(const Settings::WindowSettings& windowSettings);
    ~WindowSystem();
    void setSize(uint16_t width,uint16_t height);
    void setPosition(uint16_t x,uint16_t y);
    void show() const;
    void setTitle(const std::string& title);
    std::string getTitle() const;
    void setGraphicsWindow(osg::ref_ptr<osgViewer::GraphicsWindow> graphicsWindow);
    void setViewport(osg::ref_ptr<osg::Viewport> viewport);
    osg::ref_ptr<Resources::RenderTexture> getScreenTexture();
    
private:
    std::string m_title;
    std::pair<uint16_t, uint16_t> m_size;
    std::pair<int16_t, int16_t> m_position;
    bool m_fullscreen;
    bool m_decorated;
    uint32_t m_samples;

    void createWindow(const Settings::WindowSettings& windowSettings);

    osg::ref_ptr<osgViewer::GraphicsWindow> m_graphicsWindow = nullptr;
    osg::ref_ptr<osg::Viewport> m_viewport = nullptr;
    osg::GraphicsContext::ScreenSettings m_screenSettings;
    osg::ref_ptr<Resources::RenderTexture> m_screenTexture;
    
};
}