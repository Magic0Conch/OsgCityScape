#pragma once
#include "Render/Entities/ShadowMapBakedGeometry.h"
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
class ShadowMapBakedRender : public osg::Geometry
{
public:
    ShadowMapBakedRender(ShadowMapBakedGeometry*_generator);
    osg::Program* createRenderShader();
};

}

