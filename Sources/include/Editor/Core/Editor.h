#include <osgViewer/Viewer>

namespace CSEditor::Core {
class Editor{
public:
    Editor();
    ~Editor();
    void run();
    void preTick();
    void tick();
    void postTick();
    void logicTick();
    void renderTick();
    void setUpUI();
private:
    osgViewer::Viewer m_viewer;
    
};
}