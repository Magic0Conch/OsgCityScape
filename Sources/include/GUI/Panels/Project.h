#include <imgui.h>
#include <string>
#include <vector>
#include "Editor/Core/RuntimeContext.h"
#include "GUI/Core/Panel.h"

namespace CSEditor::GUI {
class Project : public Panel{
public:
    Project();
protected:
    virtual void drawImpl() override;
private:
    std::vector<std::string> m_folders;
};
}