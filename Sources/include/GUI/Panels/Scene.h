#include <imgui.h>
#include <memory>
#include <utility>
#include "Editor/Core/RuntimeContext.h"
#include "GUI/Core/Panel.h"


namespace CSEditor::GUI {
class Scene : public Panel{
public:
    Scene();
    std::pair<short, short> getScenePositon() const;
protected:
    virtual void drawImpl() override;
private:
    std::pair<short, short> m_scenePosition = std::pair<short, short>(-1,-1);
};
}