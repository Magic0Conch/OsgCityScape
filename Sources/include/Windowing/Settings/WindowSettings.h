#include "string"

namespace CSEditor::Settings {
struct WindowSettings{
    static const int32_t DontCare = -1;
    std::string title;
    uint16_t width;
    uint16_t height;
    bool fullscreen = false;
    bool decorated = true;
    bool resizable = true;
    int32_t refreshRate = WindowSettings::DontCare;
    uint32_t samples = 4;

    /**
    * Specifies whether the windowed mode window will be given input focus when created. This hint is ignored for
    * full screen and initially hidden windows.
    */
    bool focused = true;

    /**
    * Specifies whether the windowed mode window will be maximized when created. This hint is ignored for full screen windows.
    */
    bool maximized = false;

    /**
    * Specifies whether the windowed mode window will be floating above other regular windows, also called topmost or always-on-top.
    * This is intended primarily for debugging purposes and cannot be used to implement proper full screen windows. This hint is
    * ignored for full screen windows.
    */
    bool floating = false;


    /**
    * Specifies the desired refresh rate for full screen windows. If set to WindowSettings::DontCare, the highest
    * available refresh rate will be used. This hint is ignored for windowed mode windows.
    */

    /**
    * Specifies the default cursor mode of the window
    */
    // Cursor::ECursorMode cursorMode = Cursor::ECursorMode::NORMAL;

    /**
    * Specifies the default cursor shape of the window
    */
    // Cursor::ECursorShape cursorShape = Cursor::ECursorShape::ARROW;


};
}