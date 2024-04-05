#pragma once
#include "osg/FrameBufferObject"
#include "osg/Matrixd"
#include "osg/ref_ptr"
#include <osg/Camera>
#include <osg/Texture2D>
namespace CSEditor::Render {
class TextureProjectionPass : public osg::Camera
{
public:
    TextureProjectionPass();

    // void setTexture(osg::Texture2D * tex);
    void setTextureArray(std::vector<osg::Texture2D*>  depthTexVec, std::vector<osg::Texture2D *> colorTexVec, std::vector<osg::Matrixd*> projectionMatrix);
private:
   osg::ref_ptr<osg::Texture2DArray> m_depthMap;
   osg::ref_ptr<osg::Texture2DArray> m_colorMap;
   osg::ref_ptr<osg::Uniform> m_lightSpaceMatrixUniform;
   unsigned int m_mapIndex = 0;
};
}