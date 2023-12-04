#pragma once
#include "string"

namespace CSEditor::Settings {
struct WindowSettings{
    // static const int32_t DontCare = -1;
    std::string title = "City Scape Editor";
    uint16_t width = 720;
    uint16_t height = 1080;
    bool fullscreen = false;
    bool decorated = true;
    uint32_t samples = 4;
    // Cursor::ECursorMode cursorMode = Cursor::ECursorMode::NORMAL;
    // Cursor::ECursorShape cursorShape = Cursor::ECursorShape::ARROW;
};
}