#pragma once
#include <Windows.h>
#include <osgViewer/ViewerEventHandlers>

namespace osg {
class Camera;
}

namespace CSEditor::GUI {
class UIManager : public osgGA::GUIEventHandler
{
public:
    UIManager();

    bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) override;

protected:
    // Put your ImGui code inside this function
    virtual void drawUi() = 0;

private:
    void init();

    void setCameraCallbacks(osg::Camera* camera);

    void newFrame(osg::RenderInfo& renderInfo);

    void render(osg::RenderInfo& renderInfo);

private:
    struct ImGuiNewFrameCallback;
    struct ImGuiRenderCallback;

    double time_;
    bool mousePressed_[3];
    float mouseWheel_;
    bool initialized_;
};
}
