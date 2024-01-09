#include <imgui.h>
#include "GUI/Core/Panel.h"

namespace CSEditor::GUI {
class Inspector : public Panel{
protected:
    virtual void drawImpl() override;
};
}
