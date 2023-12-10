#pragma once
#include "Editor.h"
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

// namespace CSEditor::Core {
// extern RuntimeContext g_runtimeContext;
// }

/*




*/