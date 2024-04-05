#include "Render/LowRender/RenderDepthToTexture.h"
#include "osg/StateSet"

namespace CSEditor::Render {

#include <osg/Shader>
#include <osg/Program>


RenderDepthToTexture::RenderDepthToTexture()
{
    setClearMask(GL_DEPTH_BUFFER_BIT);
    setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
    setRenderOrder(osg::Camera::PRE_RENDER);
    setName("RenderDepth");

    _texture = new osg::Texture2D();
    _texture->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);
    _texture->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);
    _texture->setInternalFormat(GL_DEPTH_COMPONENT);
    _texture->setSourceFormat(GL_DEPTH_COMPONENT);
    _texture->setSourceType(GL_FLOAT);
    attach(osg::Camera::DEPTH_BUFFER, _texture);
    // createDepthVisualizationShader(0.1, 1000, getOrCreateStateSet());
}

osg::Texture2D* RenderDepthToTexture::getTexture() const
{
    return _texture.get();
}

} // namespace CSEditor::Render