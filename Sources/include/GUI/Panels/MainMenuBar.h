#include <imgui.h>
#include "GUI/Core/Panel.h"

namespace CSEditor::GUI {
class MainMenuBar : public Panel{
public:
    MainMenuBar();
protected:
    virtual void drawImpl() override;

};
}
