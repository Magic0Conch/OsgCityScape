#ifndef RTTCAMERA_H
#define RTTCAMERA_H
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>
#include <windows.h>
#include <osg/Camera>
#include <osg/Geode>
#include <osg/Group>
#include <osg/Referenced>
#include <osg/Uniform>
#include <osg/Vec4f>
#include <osg/ref_ptr>
#include "Resources/Material.h"
#include "Resources/RenderTexture.h"
#include "osg/Geometry"
#include "osg/StateSet"
#include "osg/Vec2"
#include "osg/Vec3"
#include "osg/Vec4"
namespace CSEditor::Render{
using namespace Resources;
class RTTCamera:public Render::Material{
private:
    osg::ref_ptr<RenderTexture> m_sourceTexture = nullptr;
    osg::ref_ptr<RenderTexture> m_destinationTexture = nullptr;
    osg::ref_ptr<osg::Camera> m_camera;
    osg::ref_ptr<osg::Geode> m_sourceGeode;
    void initializeRTTCameraParameters();
    void setDefaultUniformParameters();

public:
    RTTCamera(RenderTexture* source,RenderTexture* destination,const std::string& vertPath,const std::string& fragPath);
    RTTCamera(osg::Group* source,RenderTexture* destination,const std::string& vertPath,const std::string& fragPath);
    RTTCamera(RenderTexture* destination):m_destinationTexture(destination){
        m_camera = new osg::Camera();
        initializeRTTCameraParameters();
    }
    osg::ref_ptr<RenderTexture> getDestinationTexture() const;
    osg::ref_ptr<osg::Geode> getDestinationQuadGeode() const;
    osg::ref_ptr<osg::Camera> getRTTCamera() const;
    void addChildToRTTCamera(osg::Group* rhs);
    osg::ref_ptr<osg::StateSet> getDestinationQuadStateSet() const;

    // void renderToImage(RenderTexture* source,RenderTexture* destination){
    //     m_camera->setViewport(0, 0, destination->getTextureWidth(), destination->getTextureHeight());
    //     m_camera->attach(osg::Camera::COLOR_BUFFER0, destination);
    //     ShaderUtils::setShaderProgram(m_sourceGeode->getOrCreateStateSet(),m_vertShaderPath, m_fragShaderPath);
    //     m_sourceGeode = new osg::Geode;
    //     m_sourceGeode->addDrawable(BaseGeometryFactory::createBaseGeometry(BaseGeometryType::QUAD));
    //     if(m_sourceTexture){
    //         m_sourceGeode->getOrCreateStateSet()->setTextureAttributeAndModes(0,source,
    //         osg::StateAttribute::ON| osg::StateAttribute::OVERRIDE);
    //     }
    //     m_sourceGeode->getOrCreateStateSet()->addUniform(new osg::Uniform("_MainTex",0));
    //     m_camera->addChild(m_sourceGeode);
    // }
};
}

#endif