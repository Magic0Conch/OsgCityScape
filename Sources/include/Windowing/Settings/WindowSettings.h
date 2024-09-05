#pragma once
#include "string"
#include <osgViewer/Viewer>
namespace CSEditor::Settings {
struct WindowSettings{
    // static const int32_t DontCare = -1;
    std::string title = "City Scape Editor";
    uint16_t width = 1280;
    uint16_t height = 720;
    bool fullscreen = true;
    bool decorated = true;
    uint32_t samples = 4;
    // Cursor::ECursorMode cursorMode = Cursor::ECursorMode::NORMAL;
    // Cursor::ECursorShape cursorShape = Cursor::ECursorShape::ARROW;
    osg::GraphicsContext::ScreenSettings resolution;
};
}