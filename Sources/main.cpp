#include "Editor/Core/Application.h"
#include "osgDB/ReadFile"



int main(){
    CSEditor::Core::Application app;
    app.run();
    return 0;
}

// #include <osgViewer/Viewer>

// int main() {
//     osgViewer::Viewer viewer;

//     osg::ref_ptr<osg::Group> root = new osg::Group();
//     viewer.setSceneData(root.get());
// viewer.setUpViewInWindow(100, 100, 1280, 720);
//     // 重要：必须调用 realize 和 run 或 frame 循环
//     viewer.realize();

//     while (!viewer.done()) {
//         viewer.frame();  // 保持主循环
//     }

//     return 0;
// }
