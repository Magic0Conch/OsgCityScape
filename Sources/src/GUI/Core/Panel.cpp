#include "GUI/Core/Panel.h"
#include <iostream>
#include <osg/Camera>
#include <osgUtil/GLObjectsVisitor>
#include <osgUtil/SceneView>
#include <osgUtil/UpdateVisitor>
#include <osgViewer/ViewerEventHandlers>

#include "imgui.h"
#include "imgui_impl_opengl3.h"

using namespace CSEditor::GUI;


void Panel::drawUi(){
    drawImpl();
}

void Panel::drawImpl(){
}

