#pragma once

#include "Settings/WindowSettings.h"
#include "osg/Camera"
#include "osg/Group"
#include "osg/Texture2D"
#include "osg/Viewport"
#include "osgViewer/View"
#include "osg/ref_ptr"
#include <memory>
#include <stdint.h>
#include <osg/GraphicsContext>
#include <utility>
#include "Resources/RenderTexture.h"
#include <osgViewer/api/Win32/GraphicsWindowWin32>
#include <windows.h>

namespace CSEditor::Windowing {

class WindowSystem{
public:
    WindowSystem(Settings::WindowSettings& windowSettings);
    ~WindowSystem();
    void setSize(uint16_t width,uint16_t height);
    void setPosition(uint16_t x,uint16_t y);
    void show() const;
    void setTitle(const std::string& title);
    void setWindowIcon(osgViewer::GraphicsWindowWin32* gw, const std::string& iconPath);
    std::string getTitle() const;
    void setGraphicsWindow(osg::ref_ptr<osgViewer::GraphicsWindow> graphicsWindow);
    void setViewport(osg::ref_ptr<osg::Viewport> viewport);
    osg::ref_ptr<osg::Texture2D> getScreenTexture();
    void setScreenTexture(osg::ref_ptr<osg::Texture2D> screenTexture);
    void updateViewportSize(int width,int height);
    osg::ref_ptr<osg::GraphicsContext> getGraphicsContext() const;
    osg::ref_ptr<osg::Camera> getMainCamera() const;
    osg::ref_ptr<osg::Viewport> getViewport() const;
    std::pair<uint16_t, uint16_t> getSize() const;
private:
    std::string m_title;
    std::pair<uint16_t, uint16_t> m_size;
    std::pair<int16_t, int16_t> m_position;
    bool m_fullscreen;
    bool m_decorated;
    uint32_t m_samples;

    void createWindow(Settings::WindowSettings& windowSettings);

    osg::ref_ptr<osg::Camera> m_mainCamera;
    osg::ref_ptr<osgViewer::GraphicsWindow> m_graphicsWindow = nullptr;
    osg::ref_ptr<osg::Viewport> m_viewport = nullptr;
    osg::GraphicsContext::ScreenSettings m_screenSettings;
    osg::ref_ptr<osg::Texture2D> m_screenTexture;
    osg::ref_ptr<osg::Group> m_rootNode;
    osg::ref_ptr<osg::GraphicsContext> m_graphicsContext;
    
};
}