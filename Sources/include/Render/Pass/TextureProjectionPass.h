#pragma once
#include "osg/FrameBufferObject"
#include "osg/Matrixd"
#include "osg/Texture2DArray"
#include "osg/ref_ptr"
#include <osg/Camera>
#include <osg/Texture2D>
namespace CSEditor::Render {
class TextureProjectionPass
{
public:
    TextureProjectionPass(osg::ref_ptr<osg::Camera> camera);

    void setTexture(osg::Texture2D * tex);
    void setTextureArray(osg::ref_ptr<osg::Texture2DArray> depthMapArray, std::vector<osg::Texture2D *> colorTexVec, std::vector<osg::Matrixd*> projectionMatrix);
    
private:
    osg::ref_ptr<osg::Camera> m_camera;
    osg::ref_ptr<osg::Texture2DArray> m_colorMap;
    osg::ref_ptr<osg::Uniform> m_lightSpaceMatrixUniform;
    osg::ref_ptr<osg::Texture2D> _texture;
     unsigned int m_mapIndex = 0;
};
}