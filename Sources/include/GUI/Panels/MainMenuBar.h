#include <imgui.h>
#include "GUI/Core/Panel.h"

namespace CSEditor::GUI {
class MainMenuBar : public Panel{
public:
    MainMenuBar();
protected:
    virtual void drawImpl() override;
private:
    bool m_showImportWindow = false;
    bool m_showFlyToWindow = false;
    std::string m_inputFolderPath;
    std::string m_outputFolderPath;
    osg::Vec3f flyToPosition;
    osg::Vec3f lookAtPosition;
    void showImportWindow();
    void showFlyToWindow();
};
}
