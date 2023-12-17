#pragma once
#include "Editor.h"
#include "Editor/Core/RuntimeContext.h"
#include "RuntimeContext.h"
namespace CSEditor::Core{
class Application{
public:
    Application();
    ~Application();
    void run();
private:
    Editor m_editor;
};

}
