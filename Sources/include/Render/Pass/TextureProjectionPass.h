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
class TextureProjectionPass
{
public:
    TextureProjectionPass();
    
    void setup(osg::ref_ptr<osg::Camera> camera,osg::ref_ptr<osg::Texture2D> colorTexture,
    osg::ref_ptr<osg::Texture2D> depthStencilTexture,unsigned int cullMask,int renderOrder);
    void setProjectionMatrix(const osg::Matrixd& projectionMatrix);
    void setLightSpaceMatrixUniform(int index, osg::Matrixd& lightSpaceMatrix);
    void setTexture(int index ,osg::ref_ptr<osg::Texture2D> tex);
    void setTexture(int index ,osg::ref_ptr<osg::Image> img);
    void setProjectionEnabled(int index, bool enable);
    void setTextureArray(osg::ref_ptr<osg::Texture2DArray> depthMapArray, std::vector<osg::ref_ptr<osg::Texture2D>> colorTexVec, std::vector<osg::Matrixd>& projectionMatrix);
    void saveTexture(std::string savePath);
    void createTextureProjectionShader(const std::string& shaderName);
    osg::ref_ptr<osg::Texture2D> getColorTexture();
    osg::ref_ptr<osg::Texture2D> getDepthStencilTexture();
    
private:
    osg::ref_ptr<osg::Camera> m_camera;
    osg::ref_ptr<osg::Texture2DArray> m_colorMap;
    osg::ref_ptr<osg::Uniform> m_lightSpaceMatrixUniform;
    osg::ref_ptr<osg::Uniform> m_enableProjectionUniform;
    osg::ref_ptr<osg::Texture2D> _texture;
    osg::ref_ptr<osg::Texture2D> _depthStencilTexture;
    osg::ref_ptr<osg::Image> m_image;
    osg::Uniform* projectionUniform = new osg::Uniform(osg::Uniform::FLOAT_MAT4,"_ProjectionMatrix");
    unsigned int m_mapIndex = 0;
};
}