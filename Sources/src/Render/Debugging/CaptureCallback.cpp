#include "Render/Debugging/CaptureCallback.h"
#include <filesystem>
using namespace CSEditor::Render;

CaptureCallback::CaptureCallback(std::shared_ptr<int> other,osg::ref_ptr<osg::Texture2D> otherColorTexture)
{
    _image = new osg::Image;
    captureFlag = other;
    colorTexture = otherColorTexture;
}

CaptureCallback::~CaptureCallback() {}

void CaptureCallback::operator()(osg::RenderInfo& renderInfo) const
{
    auto ext = osg::GLExtensions::Get(renderInfo.getState()->getContextID(), true);
    if(!isInit){
        isInit = true;
        ext->glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    if(*captureFlag==0){
        OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
        auto textureObjectID = colorTexture->getTextureObject(0)->id();
        auto textureHeight = colorTexture->getTextureHeight();
        auto textureWidth = colorTexture->getTextureWidth();
        _image->allocateImage(textureWidth,textureHeight, 1, GL_RGBA, GL_UNSIGNED_BYTE);

        ext->glBindFramebuffer(GL_FRAMEBUFFER, textureObjectID);
        _image->readPixels(0, 0, textureWidth, textureHeight,  GL_RGB, GL_UNSIGNED_BYTE);
        saveImage();
        // osgDB::writeImageFile(*_image, u8"./SceneImage.jpg");
        ++(*captureFlag);
    }
}

bool CaptureCallback::readyToCapture() const{
    return *captureFlag!=0;
}

void CaptureCallback::saveImage() const{
    std::filesystem::path fullPath = std::filesystem::path(directoryPath) / fileName;
    std::filesystem::path dirPath = fullPath.parent_path();

    if (!std::filesystem::exists(dirPath)) {
        if (!std::filesystem::create_directories(dirPath)) {
            std::cerr << "Failed to create directory: " << dirPath.string() << std::endl;
            return;
        }
    }

    if (osgDB::writeImageFile(*_image, fullPath.string())) {
        std::cout << "Image saved successfully to: " << fullPath.string() << std::endl;
    } else {
        std::cerr << "Failed to save image to: " << fullPath.string() << std::endl;
    }    
}