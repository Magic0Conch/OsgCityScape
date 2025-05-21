#pragma once

#include <osg/Camera>
#include <osg/Texture2D>
#include <osg/Uniform>
#include <osg/GraphicsContext>

namespace CSEditor::Render {

class TrianglePass : public osg::Camera
{
public:
    TrianglePass();
    virtual ~TrianglePass();

    // 渲染到renderTargetTexture，渲染内容来自sceneNode，采样inputTexture
    void setup(osg::ref_ptr<osg::GraphicsContext> gc,
               int width, int height,               
               osg::ref_ptr<osg::Texture2D> renderTargetTexture,
               osg::ref_ptr<osg::Texture2D> depthStencilTexture,  // 新增参数
               unsigned int cullMask,
               int renderOrder);

    void setProjectionMatrix(const osg::Matrixd& proj);

protected:
    osg::ref_ptr<osg::Uniform> _projectionUniform;
};

}
