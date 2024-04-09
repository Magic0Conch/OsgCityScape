// #include "RenderColorToTexture.h"
#include "Render/LowRender/RenderColorToTexture.h"

using namespace CSEditor::Render;

RenderColorToTexture::RenderColorToTexture()
{
    setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
    setRenderOrder(osg::Camera::PRE_RENDER);
    setName("RenderColor");

    _texture = new osg::Texture2D();
    _texture->setWrap(osg::Texture2D::WrapParameter::WRAP_T,osg::Texture2D::WrapMode::REPEAT);
    _texture->setWrap(osg::Texture2D::WrapParameter::WRAP_S,osg::Texture2D::WrapMode::REPEAT);
    _texture->setSourceFormat(GL_RGBA);
    _texture->setInternalFormat(GL_RGBA32F_ARB);
    _texture->setSourceType(GL_FLOAT);
    attach(osg::Camera::COLOR_BUFFER0, _texture);
}
