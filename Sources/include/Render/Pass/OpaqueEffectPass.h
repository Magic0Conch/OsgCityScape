#pragma once
#include "osg/GraphicsContext"
#include "osg/Matrixd"
#include "osg/Texture2D"
#include <memory>
#include <osg/Camera>

namespace CSEditor::Render {
class OpaqueEffectPass:public osg::Camera{
public:
    OpaqueEffectPass() = default;
    void setup(osg::ref_ptr<osg::GraphicsContext> gc,osg::ref_ptr<osg::Viewport> viewport,osg::ref_ptr<osg::Texture2D> colorTexture,
    osg::ref_ptr<osg::Texture2D> depthStencilTexture,unsigned int cullMask,int renderOrder);

private:
    std::shared_ptr<osg::Matrixd> m_projectionMatrix;
};
}
