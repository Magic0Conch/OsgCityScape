#include <imgui.h>
#include "GUI/Core/Panel.h"

namespace CSEditor::GUI {
class StatusBar : public Panel{
public:
    StatusBar();
protected:
    virtual void drawImpl() override;
};
}
