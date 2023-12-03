#include "Settings/WindowSettings.h"
#include <stdint.h>

namespace CSEditor::Windowing {
class Window{
public:
    Window(const Settings::WindowSettings& windowSettings);
    ~Window();
    void setSize(uint16_t width,uint16_t height);
    void setPosition(uint16_t x,uint16_t y);
    void show() const;
    void setTitle(const std::string& p_title);
    std::string getTitle() const;

private:
    std::pair<uint16_t, uint16_t> m_size;
    std::pair<int16_t, int16_t> m_minimumSize;
    std::pair<int16_t, int16_t> m_maximumSize;
    std::pair<int16_t, int16_t> m_position;
    bool m_fullscreen;
    int32_t m_refreshRate;
};
}