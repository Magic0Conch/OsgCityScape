#include "Editor/Core/Editor.h"
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
using namespace CSEditor::Core;

osg::ref_ptr<osg::GraphicsContext> setupGC(){
    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits();
    traits->x = 20; traits->y = 30;
    traits->width = 1080; traits->height = 720;
    traits->windowDecoration = true;
    traits->doubleBuffer = true;
    traits->glContextProfileMask = 0x1;// 0x1;// 
    osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());
    if(!gc.valid()){
        osg::notify(osg::FATAL) << "Unable to create OpenGL v" << " context." << std::endl;
        return nullptr;
    }
    gc->getState()->resetVertexAttributeAlias(false);
    gc->getState()->setCheckForGLErrors(osg::State::CheckForGLErrors::ONCE_PER_ATTRIBUTE);
    return gc;
}

Editor::Editor(RuntimeContext& rtcontext):m_rtcontext(rtcontext){
    setUpUI();
    osg::Camera* cam = m_viewer.getCamera();
    auto gc = setupGC();
    // osgViewer::GraphicsWindow
    cam->setGraphicsContext(gc.get());
    cam->setViewport(new osg::Viewport( 0, 0, 540, 360));
    // m_viewer.setUpViewOnSingleScreen(0);

}

Editor::~Editor(){

}

void Editor::run(){
    
    if (!m_viewer.isRealized()){
        m_viewer.realize();
    }
    double frameTime = 0.02;
    while(!m_viewer.done())
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
    // m_viewer.getCamera()->getGraphicsContext();
}

void Editor::postTick(){

}

void Editor::logicTick(float frameTime){
    osg::Camera* cam = m_viewer.getCamera();
    
    auto viewport = cam->getViewport();

    // viewport->setViewport(0, 0, 100+frameTime*100, 100+frameTime*100);
    osgViewer::Viewer::Windows windows;
    m_viewer.getWindows(windows);
    auto window = windows.front();
    
}


void Editor::renderTick(float frameTime){

    m_viewer.frame();

    // static auto& sceneview = rtcontext_.uimanager_->GetPanel<SEditor::Panels::SceneView>("Scene View");
    // auto& testview = rtcontext_.uimanager_->GetPanel<SEditor::Panels::TestView>("Test View");
    // auto& cameraview = rtcontext_.uimanager_->GetPanel<Panels::CameraView>("Camera View");
    // sceneview.RenderTick(deltat);
    // testview.RenderTick(deltat);
    // cameraview.RenderTick(deltat);
    // rtcontext_.uimanager_->RenderTick(deltat);

}

void Editor::setUpUI(){

}
