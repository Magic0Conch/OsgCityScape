#pragma once
#include <iostream>
#include <osg/RenderInfo>
#include <osg/Texture2D>
#include <string>
#include <osgDB/WriteFile>
#include "osg/GL2Extensions"
#include "osg/GLExtensions"
#include "osg/ref_ptr"


namespace CSEditor::Render {
struct CaptureCallback :public osg::Camera::DrawCallback
{
    CaptureCallback(std::shared_ptr<int> other,osg::ref_ptr<osg::Texture2D> otherColorTexture)
    {
        _image = new osg::Image;
        // osg::ref_ptr<osg::GLExtensions> exts = osg::getGLExtensions();
        captureFlag = other;
        colorTexture = otherColorTexture;
    }
    ~CaptureCallback() {
        // glDeleteFramebuffers(1, &fboID);
    }

    virtual void operator()(osg::RenderInfo& renderInfo) const
    {
        auto ext = osg::GLExtensions::Get(renderInfo.getState()->getContextID(), true);
        if(!isInit){
            isInit = true;
            // ext->glGenFramebuffers(1, fboID);
            // ext->glBindFramebuffer(GL_FRAMEBUFFER, *fboID);
            // ext->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture->getTextureObject(0)->id(), 0);
            ext->glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        if(*captureFlag==0){
            OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
            // auto& camera = *renderInfo.getCurrentCamera();
            // auto traits = camera.getGraphicsContext()->getTraits();
            auto textureObjectID = colorTexture->getTextureObject(0)->id();
            auto textureHeight = colorTexture->getTextureHeight();
            auto textureWidth = colorTexture->getTextureWidth();
            _image->allocateImage(textureWidth,textureHeight, 1, GL_RGBA, GL_UNSIGNED_BYTE);

            ext->glBindFramebuffer(GL_FRAMEBUFFER, textureObjectID);
            // glReadBuffer(GL_COLOR_ATTACHMENT0);
            _image->readPixels(0, 0, textureWidth, textureHeight,  GL_RGB, GL_UNSIGNED_BYTE);
            osgDB::writeImageFile(*_image, u8"./SceneImage.jpg");
            ++(*captureFlag);
        }
    }
    osg::Vec2i pos;
    osg::Vec2i size;
    mutable bool isInit = false;
    GLuint* fboID;
    osg::ref_ptr<osg::Image> _image;
    osg::ref_ptr<osg::Texture2D> colorTexture;
    std::shared_ptr<int> captureFlag;
    mutable OpenThreads::Mutex  _mutex;
};

}