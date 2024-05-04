#pragma once
#include <osg/GL2Extensions>
#include <osg/Notify>
#include <osg/ref_ptr>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Point>
#include <osg/Vec3>
#include <osg/Vec4>
#include <osg/Program>
#include <osg/Shader>
#include <osg/BlendFunc>

#include <osg/Uniform>
#include <osgViewer/Viewer>

#include <osg/BufferIndexBinding>

namespace CSEditor::Render {
class ShadowMapBakedGeometry:public osg::Geometry
{
public:
    ShadowMapBakedGeometry();
    void init();
    void setRenderer(osg::Geometry*renderer);
    GLuint getNumPrimitivesGenerated()const;
    void createTextureProjectionShader(osg::StateSet* ss);
protected:
    osg::ref_ptr<osg::VertexBufferObject> genbuffer;//Renderer buffer
    osg::Vec4Array* vAry;

    virtual void drawImplementation( osg::RenderInfo& renderInfo ) const;
};

}
