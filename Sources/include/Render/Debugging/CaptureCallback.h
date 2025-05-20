#pragma once
#include <osg/RenderInfo>
#include <osg/Texture2D>
#include <osgDB/WriteFile>
#include "osg/ref_ptr"


namespace CSEditor::Render {
class CaptureCallback :public osg::Camera::DrawCallback
{
public:
    CaptureCallback(std::shared_ptr<int> other,osg::ref_ptr<osg::Texture2D> otherColorTexture);
    ~CaptureCallback();
    virtual void operator()(osg::RenderInfo& renderInfo) const;
    bool readyToCapture() const;

    std::string directoryPath;
    std::string fileName;
private:
    void saveImage() const;
    mutable bool isInit = false;
    osg::ref_ptr<osg::Image> _image;
    osg::ref_ptr<osg::Texture2D> colorTexture;
    std::shared_ptr<int> captureFlag;
    mutable OpenThreads::Mutex  _mutex;

};

}