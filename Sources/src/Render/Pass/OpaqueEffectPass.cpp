#include "Render/Pass/OpaqueEffectPass.h"

using namespace CSEditor::Render;

void OpaqueEffectPass::setup(osg::ref_ptr<osg::GraphicsContext> gc,osg::ref_ptr<osg::Viewport> viewport,osg::ref_ptr<osg::Texture2D> colorTexture,
osg::ref_ptr<osg::Texture2D> depthStencilTexture,unsigned int cullMask,int renderOrder){
    setGraphicsContext(gc);
    setViewport(viewport);
    setCullMask(cullMask);
    // setCullingMode(m_mainCamera->getCullingMode() & ~osg::CullSettings::SMALL_FEATURE_CULLING);
    setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
    attach(osg::Camera::COLOR_BUFFER0,colorTexture);
    attach(osg::Camera::PACKED_DEPTH_STENCIL_BUFFER,depthStencilTexture);
    setClearMask(GL_NONE);
    setRenderOrder(osg::Camera::PRE_RENDER,renderOrder);
}