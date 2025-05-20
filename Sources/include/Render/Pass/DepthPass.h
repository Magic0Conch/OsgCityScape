#pragma once

#include "osg/Matrixd"
#include "osg/Texture2DArray"
#include <osg/Camera>
#include <osg/Texture2D>

namespace CSEditor::Render {

class DepthPass : public osg::Camera
{
public:
    DepthPass();
    void setup(osg::ref_ptr<osg::GraphicsContext> gc,const int width,const int height,osg::ref_ptr<osg::Texture2DArray> depthArray,
    int depthTextureIndex,unsigned int cullMask,int renderOrder);
    virtual ~DepthPass();
    osg::Texture2D* getTexture() const;
    void setViewMatrix(const osg::Matrixd& viewMatrix);
    void setProjectionMatrix(const osg::Matrixd& projectionMatrix);
private:
    void createDepthShader(osg::StateSet* ss);
private:
    osg::ref_ptr<osg::Texture2D> _texture;
};

}