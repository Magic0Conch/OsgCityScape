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
#include "osg/Image"
#include "osg/StateSet"
#include "osg/Texture2D"
#include "osg/Vec2"
#include "osg/Vec3"
#include "osg/Vec4"
namespace CSEditor::Render{
using namespace Resources;
class RTTCamera:public osg::Referenced{

public:
    /*constructor*/
    RTTCamera(osg::Texture2D* source,osg::Texture2D* destination,const std::string& vertPath,const std::string& fragPath);
    RTTCamera(osg::Texture2D* source,osg::Image* destination,const std::string& vertPath,const std::string& fragPath);
    RTTCamera(osg::Group* source,osg::Texture2D* destination,const std::string& vertPath,const std::string& fragPath);
    RTTCamera(osg::Texture2D* destination);
    RTTCamera(osg::Texture2D* destination,osg::ref_ptr<Material> material);

    /*setter and getter*/
    osg::ref_ptr<osg::Texture2D> getDestinationTexture() const;
    osg::ref_ptr<osg::Geode> getDestinationQuadGeode() const;
    osg::ref_ptr<osg::Camera> getRTTCamera() const;
    osg::ref_ptr<Render::Material> getMaterial() const;
    void addChildToRTTCamera(osg::Group* rhs);
    osg::ref_ptr<osg::StateSet> getDestinationQuadStateSet() const;
    void setMutisample(bool flag);

    // void renderToImage(osg::Texture2D* source,osg::Texture2D* destination,Material* material=nullptr){
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
private:
    osg::ref_ptr<Render::Material> m_material;
    osg::ref_ptr<osg::Texture2D> m_sourceTexture = nullptr;
    osg::ref_ptr<osg::Texture2D> m_destinationTexture = nullptr;
    osg::ref_ptr<osg::Image> m_destinationImage = nullptr;
    osg::ref_ptr<osg::Camera> m_camera;
    osg::ref_ptr<osg::Geode> m_sourceGeode;
    bool m_mutisample = false;
    void initializeRTTCameraParameters();
    void setDefaultUniformParameters();

};
}

#endif