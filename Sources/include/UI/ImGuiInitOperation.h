#ifndef IMGUI_INIT_OPERATION_H
#define IMGUI_INIT_OPERATION_H
#include <Windows.h>
#include <osgViewer/ViewerEventHandlers>
#include <imgui_impl_opengl3.h>
#include <iostream>
namespace cs {
    class ImGuiInitOperation : public osg::Operation
    {
    public:
        ImGuiInitOperation();
        void operator()(osg::Object* object) override;
    };
}
#endif