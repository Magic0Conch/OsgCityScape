#pragma once
#include <osgViewer/Viewer>

namespace CSEditor::Core {
class Editor{
public:
    Editor();
    ~Editor();
    void run();
    void preTick();
    void tick(float deltaTime);
    void postTick();
    void logicTick(float deltaTime);
    void renderTick(float deltaTime);
    void setUpUI();
private:
    osgViewer::Viewer m_viewer;
};
}