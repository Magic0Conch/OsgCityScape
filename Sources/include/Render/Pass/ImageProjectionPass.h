#pragma once
#include "osg/FrameBufferObject"
#include "osg/Image"
#include "osg/Matrixd"
#include "osg/Texture2DArray"
#include "osg/Uniform"
#include "osg/ref_ptr"
#include <osg/Camera>
#include <osg/Texture2D>
#include <string>
namespace CSEditor::Render {
class ImageProjectionPass : public osg::Camera
{
public:
    ImageProjectionPass();
    
    void setup(osg::ref_ptr<osg::GraphicsContext> gc,const int width,const int height,osg::ref_ptr<osg::Texture2D> mainTexture,osg::ref_ptr<osg::Texture2D> colorTexture,osg::ref_ptr<osg::Texture2D> depthMap,
    osg::Matrixd projectorProjectionMatrix,osg::ref_ptr<osg::Texture2D> targetDepthStencilTexture,osg::ref_ptr<osg::Texture2D> targetTexture,unsigned int cullMask,int renderOrder);
    void setProjectionMatrix(const osg::Matrixd& projectionMatrix);
    void setLightSpaceMatrixUniform(int index, osg::Matrixd& lightSpaceMatrix);
    void setProjectionEnabled(int index, bool enable);
    
    void createTextureProjectionShader(const std::string& shaderName);
    osg::ref_ptr<osg::Texture2D> getColorTexture();
    osg::ref_ptr<osg::Texture2D> getDepthStencilTexture();
    
private:    
    osg::ref_ptr<osg::Texture2D> m_colorMap;
    osg::ref_ptr<osg::Uniform> m_lightSpaceMatrixUniform;
    osg::ref_ptr<osg::Uniform> m_enableProjectionUniform;
    osg::ref_ptr<osg::Texture2D> _texture;
    osg::ref_ptr<osg::Texture2D> _targetDepthStencilTexture;
    osg::ref_ptr<osg::Texture2D> _targetTexture;
    osg::ref_ptr<osg::Image> m_image;
    osg::Uniform* projectionUniform = new osg::Uniform(osg::Uniform::FLOAT_MAT4,"_ProjectionMatrix");
    unsigned int m_mapIndex = 0;
};
}