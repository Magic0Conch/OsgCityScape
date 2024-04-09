#include <imgui.h>
#include <memory>
#include "Editor/Core/RuntimeContext.h"
#include "GUI/Core/Panel.h"

namespace CSEditor::GUI {
class Scene : public Panel{
public:
    Scene();
protected:
    virtual void drawImpl() override;
private:
    
};
}