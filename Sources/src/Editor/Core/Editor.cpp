#include "Editor/Core/Editor.h"
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

Editor::Editor(){
    setUpUI();
}

Editor::~Editor(){

}

void Editor::run(){
    
    if (!m_viewer.isRealized())
    {
        m_viewer.realize();
    }
    while(!m_viewer.done())
    {
        osg::Timer_t startFrameTick = osg::Timer::instance()->tick();
        preTick();
        tick();
        postTick();
        osg::Timer_t endFrameTick = osg::Timer::instance()->tick();
        double frameTime = osg::Timer::instance()->delta_s(startFrameTick, endFrameTick);
    }

}

void Editor::preTick(){

}

void Editor::tick(){
    //render tick
    m_viewer.frame();
    
    m_viewer.getCamera()->getGraphicsContext();
}

void Editor::postTick(){

}

void Editor::logicTick(){

}

void Editor::renderTick(){

}

void Editor::setUpUI(){

}
