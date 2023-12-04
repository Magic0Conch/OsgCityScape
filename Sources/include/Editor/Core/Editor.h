#pragma once
#include <osgViewer/Viewer>
#include "RuntimeContext.h"
namespace CSEditor::Core {
class Editor{
public:
    Editor(RuntimeContext& rtcontext);
    ~Editor();
    void run();
    void preTick();
    void tick(float deltaTime);
    void postTick();
    void logicTick(float deltaTime);
    void renderTick(float deltaTime);
    void setUpUI();
private:
    RuntimeContext& m_rtcontext;
    osgViewer::Viewer m_viewer;

};
}