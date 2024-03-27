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
    std::string m_inputFolderPath;
    std::string m_outputFolderPath;
    void showImportWindow();
};
}
