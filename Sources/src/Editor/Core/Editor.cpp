#include "Editor/Core/Editor.h"
#include "Editor/Core/RuntimeContext.h"
#include "osg/PositionAttitudeTransform"
#include "osg/Vec3f"
#include "osg/ref_ptr"
#include "osgDB/ReadFile"
#include "osgViewer/GraphicsWindow"
#include <iostream>
#include <memory>
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
#include <utility>
#include "GUI/Panels/Hierachy.h"
#include "GUI/Panels/Inspector.h"
#include "GUI/Panels/Console.h"
#include "GUI/Panels/Project.h"
#include "GUI/Panels/Scene.h"
#include "GUI/Panels/MainMenuBar.h"
#include "GUI/Helper/ImGuiInitOperation.h"
#include "Render/RenderSystem.h"

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
    g_runtimeContext.renderSystem->tick(frameTime);
}

void Editor::setUpUI(){
    // g_runtimeContext.viewer->addEventHandler(new osgViewer::StatsHandler());
    g_runtimeContext.viewer->addEventHandler(Core::g_runtimeContext.uiManager.get());
    g_runtimeContext.uiManager->createPanel<GUI::Hierachy>("Hierachy");
    g_runtimeContext.uiManager->createPanel<GUI::Inspector>("Inspector");
    g_runtimeContext.uiManager->createPanel<GUI::Console>("Console");
    g_runtimeContext.uiManager->createPanel<GUI::Project>("Project");
    g_runtimeContext.uiManager->createPanel<GUI::Scene>("Scene");
    g_runtimeContext.uiManager->createPanel<GUI::MainMenuBar>("MainMenuBar");
    g_runtimeContext.viewer->setRealizeOperation(new GUI::ImGuiInitOperation);


    // auto data = CSEditor::Core::g_runtimeContext.viewer->getSceneData();
    // data->asGroup()->addChild(root);
    // CSEditor::Core::g_runtimeContext.viewer->setSceneData(pat);
    CSEditor::Core::g_runtimeContext.viewer->realize();
    g_runtimeContext.logSystem = std::make_unique<Helpers::LogSystem>();
    g_runtimeContext.logSystem->info("Hello world");
}
