#pragma once

#include <osgViewer/Viewer>
#include <osg/GraphicsContext>
#include <osg/ref_ptr>
#include <memory>
namespace CSEditor::Windowing {
    class Window;
}
namespace CSEditor::Core {
class RuntimeContext{
public:
    RuntimeContext();
    ~RuntimeContext();

    osg::ref_ptr<osgViewer::Viewer> viewer;
    std::unique_ptr<CSEditor::Windowing::Window> window;
};

extern RuntimeContext g_runtimeContext;
}