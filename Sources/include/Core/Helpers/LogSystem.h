#pragma once
#include "Editor/Core/RuntimeContext.h"
#include "GUI/Core/UIManager.h"
#include "json11.hpp"
#include "Serializer.h"
#include <memory>
#include <GUI/Panels/Console.h>
#include <string>

namespace CSEditor::Helpers{
class LogSystem{
public:
    LogSystem();
    void info(std::string&);
    void error(std::string&);
    void warn(std::string&);
    void info(std::string&&);
    void error(std::string&&);
    void warn(std::string&&);
private:
    GUI::Console& m_console;
    int m_counter;
};
}