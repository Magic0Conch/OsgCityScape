#include <imgui.h>
#include "Editor/Core/RuntimeContext.h"
#include "GUI/Core/Panel.h"

namespace CSEditor::GUI {
class Console : public Panel{
protected:
    virtual void drawImpl() override;
};
}