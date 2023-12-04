#pragma once
#include "Windowing/Window.h"
#include <osgViewer/Viewer>
#include <osg/GraphicsContext>
#include <osg/ref_ptr>
#include <memory>
namespace CSEditor::Core {
class RuntimeContext{
public:
    RuntimeContext();
    ~RuntimeContext();

    osg::ref_ptr<osgViewer::Viewer>  m_viewer;
    std::unique_ptr<Windowing::Window> m_window;
};
}