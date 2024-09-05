

#include "osg/Image"
#include "osg/Vec2"
#include "osgDB/WriteFile"
#include <osg/Camera>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include "Resources/Loaders/ShaderLoader.h"
#include "Core/ECS/Level.h"
#include "Resources/ResourceManagement/ConfigManager.h"
#include "Editor/Core/RuntimeContext.h"
struct CaptureCallback :public osg::Camera::DrawCallback
{
    CaptureCallback()
    {
    }
    ~CaptureCallback() {}

    virtual void operator()(osg::RenderInfo& renderInfo) const
    {
        OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
        auto& camera = *renderInfo.getCurrentCamera();
        auto traits = camera.getGraphicsContext()->getTraits();
        glReadBuffer(GL_FRONT);
        _image->readPixels(pos[0], pos[1], size[0], size[1],  GL_RGB, GL_UNSIGNED_BYTE);
        osgDB::writeImageFile(*_image, u8"./SceneImage.jpg");
    }
    osg::Vec2i pos;
    osg::Vec2i size;
    osg::observer_ptr<osg::Image> _image;
    mutable OpenThreads::Mutex  _mutex;

};

int main(int argc, char** argv) {
    osgViewer::Viewer viewer;
    viewer.setUpViewOnSingleScreen();
    const auto modelMeshPath = (CSEditor::Core::g_runtimeContext.configManager->getAssetFolder()/ "models/cow.osg").string();
    // Load a model or create a scene
    osg::ref_ptr<osg::Node> scene = osgDB::readNodeFile(modelMeshPath);
    // Load a model or create a scene
    // osg::ref_ptr<osg::Node> scene = osgDB::readNodeFile("cow.osg");
    viewer.setSceneData(scene.get());
    osg::ref_ptr<CaptureCallback> cb = new CaptureCallback();
    osg::Image* image = new osg::Image;
    cb->pos = osg::Vec2i(0,0);
    cb->size = osg::Vec2i(1920,1080);
    cb->_image = image;
    // Add the screen capture event handler
    // viewer.addUpdateOperation(new CaptureCallback);
    viewer.getCamera()->setFinalDrawCallback(cb);
    // Add a built-in screen capture handler
    // viewer.addEventHandler(new osgViewer::ScreenCaptureHandler);

    // Run the viewer
    return viewer.run();
}
