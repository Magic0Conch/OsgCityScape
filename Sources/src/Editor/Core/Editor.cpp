#include "Editor/Core/Editor.h"
#include "Editor/Core/RuntimeContext.h"
#include "osgViewer/GraphicsWindow"
#include <iostream>
#include <ostream>
#include <stdlib.h>
#include <string.h>
#include <osgViewer/Viewer>
#include <osgViewer/Renderer>

#include <osg/os_utils>
#include <osg/io_utils>

#include <osg/TextureCubeMap>
#include <osg/TextureRectangle>
#include <osg/TexMat>
#include <osg/DeleteHandler>

#include <osgDB/Registry>

#include <osgUtil/Optimizer>
#include <osgUtil/IntersectionVisitor>
#include <osgUtil/Statistics>
#include <imgui.h>
#include "GUI/Panels/Hierachy.h"
#include "GUI/Panels/Inspector.h"
#include "GUI/Helper/ImGuiInitOperation.h"

using namespace CSEditor::Core;

Editor::Editor(){
    setUpUI();
}

Editor::~Editor(){

}

void Editor::run(){
    double frameTime = 0.02;
    while(!g_runtimeContext.viewer->done())
    {
        osg::Timer_t startFrameTick = osg::Timer::instance()->tick();
        preTick();
        tick(frameTime);
        postTick();
        osg::Timer_t endFrameTick = osg::Timer::instance()->tick();
        frameTime = osg::Timer::instance()->delta_s(startFrameTick, endFrameTick);
    }
}

void Editor::preTick(){

}

void Editor::tick(float deltaTime){
    //render tick
    logicTick(deltaTime);
    renderTick(deltaTime);
}

void Editor::postTick(){

}

void Editor::logicTick(float frameTime){

}


void Editor::renderTick(float frameTime){
    g_runtimeContext.viewer->frame();
}

void Editor::setUpUI(){
    g_runtimeContext.viewer->addEventHandler(new osgViewer::StatsHandler());
    g_runtimeContext.viewer->addEventHandler(new GUI::Hierachy);
    // g_runtimeContext.viewer->addEventHandler(new GUI::Inspector);
    g_runtimeContext.viewer->setRealizeOperation(new GUI::ImGuiInitOperation);
    // g_runtimeContext.viewer->run();
    CSEditor::Core::g_runtimeContext.viewer->realize();

    
}
